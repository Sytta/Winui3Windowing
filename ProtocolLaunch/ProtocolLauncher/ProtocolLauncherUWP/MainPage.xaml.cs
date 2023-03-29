using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel.DataTransfer;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace ProtocolLauncherUWP
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        // Please change this to the package family name of the testing app, if testing other apps
        const string packageName = "17a86331-a6d4-45cd-b3b8-a2aebe4b3f24_07y260wq62y4j";

        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void launcAsyncClick(object sender, RoutedEventArgs e)
        {
            var picker = new Windows.Storage.Pickers.FileOpenPicker();

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
            var picker = new Windows.Storage.Pickers.FileOpenPicker();

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

            // For example: ms-photos:videoedit?InputToken=123abc&Action=Trim&StartTime=01:02:03

            _ = await Launcher.LaunchUriForResultsAsync(new Uri("winui3.protocol.launch:", UriKind.Absolute), options, inputData);
        }
    }
}
