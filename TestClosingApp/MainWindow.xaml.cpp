#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Windows.System.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.


namespace winrt::TestClosingApp::implementation
{
    constexpr std::wstring_view c_uri = L"ms-testclosingapp:";

    winrt::Windows::Foundation::IAsyncAction MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
        winrt::Windows::Foundation::Uri uri(c_uri);
        co_await winrt::Windows::System::Launcher::LaunchUriAsync(uri);
    }
}
