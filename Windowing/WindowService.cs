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
        private ulong _windowCounter = 0;

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

                        CppWinrtComponent.WindowService.SetCurrentWindow(newWindow);

                        var frame = new Frame();

                        AppWindow newAppWindow = CppWinrtComponent.WindowService.GetCurrentAppWindow();
                        newAppWindow.TitleBar.ExtendsContentIntoTitleBar = true;

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

                // Need to kill the process explicitly to exit. Otherwise the process keeps running. Is there a better way to do this?
                // Application.Current.Exit does not end the process.
                Process.GetCurrentProcess().Kill();
            }

            Debug.WriteLine($"[WindowCounter]: {_windowCounter}");
        }
    }
}
