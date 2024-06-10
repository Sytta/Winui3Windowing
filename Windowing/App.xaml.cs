// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;
using Microsoft.Windows.AppLifecycle;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using Windows.ApplicationModel.Activation;
using Windows.Foundation.Collections;


// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace Windowing
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    public partial class App : Application
    {
        private bool _mainThreadActivated = false;
        private WindowService _windowService;
        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            this.InitializeComponent();
            _windowService = new WindowService();
        }

        /// <summary>
        /// Invoked when the application is launched.
        /// </summary>
        /// <param name="args">Details about the launch request and process.</param>
        protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
        {
            if (_mainThreadActivated == false)
            {
                _mainThreadActivated = true;
                var appInstance = AppInstance.GetCurrent();
                // Bug WinAppSDK: appInstance.GetActivatedEventArgs() exception when Open With -> Choose another app
                _ = appInstance.GetActivatedEventArgs();
                _windowService.CreateAndActivateNewWindowAsync(typeof(Page1));
            }
        }

        public void OnActivated(object sender, AppActivationArguments args)
        {
            ExtendedActivationKind kind = args.Kind;
            if (kind == ExtendedActivationKind.Launch)
            {
                Debug.WriteLine("Activation kind: Launch");
                _windowService.CreateAndActivateNewWindowAsync(typeof(Page1));
            }
            else if (kind == ExtendedActivationKind.File)
            {
                Debug.WriteLine("Activation kind: File");
                _windowService.CreateAndActivateNewWindowAsync(typeof(CppWinrtComponent.Page2));
            }
            else if (kind == ExtendedActivationKind.Protocol || kind == ExtendedActivationKind.ProtocolForResults) 
            {
                IProtocolActivatedEventArgsWithCallerPackageFamilyNameAndData protocolArgs = (IProtocolActivatedEventArgsWithCallerPackageFamilyNameAndData)args.Data;
                ValueSet valueSet = protocolArgs.Data;
                bool hasKey = valueSet.TryGetValue("InputToken", out object name);
                if (hasKey) 
                {
                    string key = name as string;

                    IntPtr redirectEventHandle = CreateEvent(IntPtr.Zero, true, false, key);
                    SetEvent(redirectEventHandle);

                    //bool createdNew = false;
                    //var redirectWaitHandle = new EventWaitHandle(initialState: false, EventResetMode.ManualReset, name: key, out createdNew);

                    //if (!createdNew)
                    //{
                    //    redirectWaitHandle.Set();
                    //}

                }
                else
                {
                    Debug.WriteLine("unable to get it!");
                }

                /*
                 {[InputToken, 0A698AD8-EAE4-4EA8-AD9D-B37B16984D07]}
                 {[InputToken, 789EC8B6-C458-456F-BFE4-A74A2850C187]}
                {[InputToken, BC7CC68C-E45B-444E-B0BB-57B672A7C431]}
                 */
            }
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        private static extern IntPtr CreateEvent(
            IntPtr lpEventAttributes, bool bManualReset,
            bool bInitialState, string lpName);

        [DllImport("kernel32.dll")]
        private static extern bool SetEvent(IntPtr hEvent);
    }
}
