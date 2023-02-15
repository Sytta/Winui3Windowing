// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;
using Microsoft.Windows.AppLifecycle;
using System.Diagnostics;


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
            }
            else if (kind == ExtendedActivationKind.File)
            {
                Debug.WriteLine("Activation kind: File");
            }

            _windowService.CreateAndActivateNewWindowAsync(typeof(CppWinrtComponent.Page2));

        }
    }
}
