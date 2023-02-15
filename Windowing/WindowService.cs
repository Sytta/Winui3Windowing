using System.Diagnostics;
using System.Threading;

using Microsoft.UI;
using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Windowing;

#nullable enable

namespace Windowing
{
    internal class WindowService
    {
        private static ThreadLocal<Window>? _window;
        private static ThreadLocal<WindowId>? _windowId;
        private ulong _windowCounter = 0;
        private WindowId? _mainWindowId;

        public static void SetCurrentWindow(Window window)
        {
            Debug.Assert(_window == null);
            _window = new ThreadLocal<Window> { Value = window};

            Debug.Assert(_window != null && _window.IsValueCreated);
            var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(_window);
            _windowId = new ThreadLocal<WindowId> { Value = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(hwnd) };
        }

        public static Window GetCurrentWindow() 
        {
            Debug.Assert(_window != null && _window.IsValueCreated);
            return _window.Value;
        }

        public static AppWindow GetCurrentAppWindow()
        {
            Debug.Assert(_windowId != null && _windowId.IsValueCreated);
            return AppWindow.GetFromWindowId(_windowId.Value);
        }

        public static WindowId GetCurrentAppWindowId()
        {
            Debug.Assert(_windowId != null && _windowId.IsValueCreated);
            return _windowId.Value;
        }


        public void CreateAndActivateNewWindowAsync(System.Type? pageType)
        {
            var newWindowThread = new Thread(() =>
            {
                WinRT.ComWrappersSupport.InitializeComWrappers();

                Application.Start((p) =>
                {
                    var context = new DispatcherQueueSynchronizationContext(DispatcherQueue.GetForCurrentThread());
                    SynchronizationContext.SetSynchronizationContext(context);

                    DispatcherQueue.GetForCurrentThread().TryEnqueue(() =>
                    {
                        var newWindow = new MainWindow();

                        SetCurrentWindow(newWindow);

                        // Set window
                        if (_mainWindowId == null)
                        {
                            _mainWindowId = WindowService.GetCurrentAppWindowId();
                        }

                        var frame = new Frame();
                        //frame.RequestedTheme = _themeService.Theme;

                        AppWindow newAppWindow = WindowService.GetCurrentAppWindow();
                        newAppWindow.TitleBar.ExtendsContentIntoTitleBar = true;

                        //currentWindow.Activated += SingleView_Activated;
                        newWindow.Content = frame;

                        // Setup consolidation
                        newAppWindow.Closing += OnAppWindowClosing;

                        // Display the window
                        newWindow.Activate();

                        // Increase window counter
                        Interlocked.Increment(ref _windowCounter);

                        Debug.WriteLine($"[WindowCounter]: {_windowCounter}");

                        _ = frame?.Navigate(pageType);
                    });
                });
            });

            newWindowThread.SetApartmentState(ApartmentState.STA);
            newWindowThread.Start();
            // Do not wait for thread to finish. The new thread will only finish when the window on that thread closes.
        }

        private void OnAppWindowClosing(AppWindow sender, AppWindowClosingEventArgs args)
        {
            Debug.WriteLine($"OnAppWindowClosing called on {Thread.CurrentThread.ManagedThreadId}");
            sender.Closing -= OnAppWindowClosing;
            DecrementWindowCounter();
        }

        private void DecrementWindowCounter()
        {
            Debug.WriteLine($"DecrementWindowCounter called on {Thread.CurrentThread.ManagedThreadId}");
            if (Interlocked.Decrement(ref _windowCounter) == 0)
            {
                Debug.WriteLine($"Process exiting on {Thread.CurrentThread.ManagedThreadId}");
                Process.GetCurrentProcess().Kill();
            }

            Debug.WriteLine($"[WindowCounter]: {_windowCounter}");
        }
    }
}
