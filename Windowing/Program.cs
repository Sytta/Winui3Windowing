// Copyright (c) Microsoft. All rights reserved.

using Microsoft.UI.Dispatching;
using System;
using System.Threading;
using Microsoft.Windows.AppLifecycle;
using Microsoft.UI.Xaml;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using Windowing;

namespace PhotosApp
{
    public static class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            WinRT.ComWrappersSupport.InitializeComWrappers();

            bool isRedirect = DecideRedirection();
            if (!isRedirect)
            {
                Application.Start((p) =>
                {
                    var context = new DispatcherQueueSynchronizationContext(DispatcherQueue.GetForCurrentThread());
                    SynchronizationContext.SetSynchronizationContext(context);
                    new App();
                });
            }
        }

        // Decide if we want to redirect the incoming activation to another instance.
        private static bool DecideRedirection()
        {
            bool isRedirect = false;

            try
            {
                // TODO WinAppSDK: Update UniqueString
                AppInstance keyInstance = AppInstance.FindOrRegisterForKey("UniqueString");

                // If we successfully registered the key, we must be the
                // only instance running that was activated. 
                if (keyInstance.IsCurrent)
                {
                    // Hook up the Activated event, to allow for this instance of the app
                    // getting reactivated as a result of multi-instance redirection.
                    keyInstance.Activated += KeyInstance_Activated;
                }
                else
                {
                    isRedirect = true;
                    AppActivationArguments args = AppInstance.GetCurrent().GetActivatedEventArgs();
                    RedirectActivationTo(args, keyInstance);
                }
            }
            catch (Exception ex)
            {
                Trace.TraceError($"Error getting instance information: {ex.Message}");
            }

            return isRedirect;
        }

        private static void KeyInstance_Activated(object sender, AppActivationArguments e)
        {
            var currentApp = Application.Current as App;
            currentApp.OnActivated(sender, e);
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        private static extern IntPtr CreateEvent(
        IntPtr lpEventAttributes, bool bManualReset,
        bool bInitialState, string lpName);

        [DllImport("kernel32.dll")]
        private static extern bool SetEvent(IntPtr hEvent);

        [DllImport("ole32.dll")]
        private static extern uint CoWaitForMultipleObjects(
            uint dwFlags, uint dwMilliseconds, ulong nHandles,
            IntPtr[] pHandles, out uint dwIndex);

        private static IntPtr redirectEventHandle = IntPtr.Zero;

        // Do the redirection on another thread, and use a non-blocking
        // wait method to wait for the redirection to complete.
        public static void RedirectActivationTo(
            AppActivationArguments args, AppInstance keyInstance)
        {
            redirectEventHandle = CreateEvent(IntPtr.Zero, true, false, null);
            Task.Run(() =>
            {
                keyInstance.RedirectActivationToAsync(args).AsTask().Wait();
                SetEvent(redirectEventHandle);
            });
            uint CWMO_DEFAULT = 0;
            uint INFINITE = 0xFFFFFFFF;
            _ = CoWaitForMultipleObjects(
               CWMO_DEFAULT, INFINITE, 1,
               new IntPtr[] { redirectEventHandle }, out uint handleIndex);
        }
    }
}
