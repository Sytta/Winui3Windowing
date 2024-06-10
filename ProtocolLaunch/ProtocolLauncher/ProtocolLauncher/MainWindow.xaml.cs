// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;
using System;
using System.Diagnostics;
using Windows.ApplicationModel.DataTransfer;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage.Pickers;
using Windows.System;
using WinRT.Interop;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace ProtocolLauncher
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        // Please change this to the package family name of the testing app, if testing other apps
        //const string packageName = "17a86331-a6d4-45cd-b3b8-a2aebe4b3f24_07y260wq62y4j";
        const string packageName = "8f33c18e-759b-4308-aea0-d4563eff5a16_07y260wq62y4j";

        public MainWindow()
        {
            this.InitializeComponent();
        }

        private async void launcAsyncClick(object sender, RoutedEventArgs e)
        {
            var picker = new Windows.Storage.Pickers.FileOpenPicker();
            var hWnd = WindowNative.GetWindowHandle(this);
            InitializeWithWindow.Initialize(picker, hWnd);
            
            picker.ViewMode = Windows.Storage.Pickers.PickerViewMode.Thumbnail;
            picker.SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.PicturesLibrary;
            picker.FileTypeFilter.Add(".mp4");

            //Select a single file
            Windows.Storage.StorageFile file = await picker.PickSingleFileAsync();

            if (file == null)
                return;

            var inputFile = SharedStorageAccessManager.AddFile(file);
            LauncherOptions options = new LauncherOptions();
            options.TargetApplicationPackageFamilyName = packageName;

            var inputData = new ValueSet();
            inputData.Add("InputToken", inputFile);//input file

            _ = await Launcher.LaunchUriAsync(new Uri("winui3.protocol.launch:", UriKind.Absolute), options, inputData);
        }

        private async void launchForResultClick(object sender, RoutedEventArgs e) 
        {
            var inputData = new ValueSet();
            var hWnd = WindowNative.GetWindowHandle(this);

            if (true)
            {
                var picker = new Windows.Storage.Pickers.FileOpenPicker();
                InitializeWithWindow.Initialize(picker, hWnd);
                picker.ViewMode = Windows.Storage.Pickers.PickerViewMode.Thumbnail;
                picker.SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.PicturesLibrary;
                picker.FileTypeFilter.Add(".mp4");

                //Select a single file
                Windows.Storage.StorageFile file = await picker.PickSingleFileAsync();

                if (file == null)
                    return;

                var inputFile = SharedStorageAccessManager.AddFile(file);
                inputData.Add("InputToken", inputFile);//input file
            }

            LauncherOptions options = new LauncherOptions();
            options.TargetApplicationPackageFamilyName = "Microsoft.Windows.Photos_8wekyb3d8bbwe";
            InitializeWithWindow.Initialize(options, hWnd);

            inputData.Add("Action", "Trim");//action

            // For example: ms-photos:videoedit?InputToken=123abc&Action=Trim&StartTime=01:02:03

            var results = await Launcher.LaunchUriForResultsAsync(new Uri("microsoft.windows.photos.videoedit:"), options, inputData);
            var status = results.Status;
            var result = results.Result;

            Trace.Write("Operation done");
        }
    }
}
