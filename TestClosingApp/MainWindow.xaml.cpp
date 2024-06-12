#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Windows.System.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.


namespace winrt::TestClosingApp::implementation
{
    constexpr std::wstring_view c_uri = L"ms-testclosingapp:";

    winrt::Windows::Foundation::IAsyncAction MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        bool alreadyExisting = false;
        if (!m_semaphoreAcquired)
        {
            bool created = m_semaphore.try_create(0, INT_MAX, L"TestClosingAppSemaphore", SEMAPHORE_ALL_ACCESS, nullptr, &alreadyExisting);
            assert(created);
            m_semaphore.acquire();
            m_semaphoreAcquired = true;
        }

        winrt::hstring firstProcessString = alreadyExisting ? L"Not first process" : L"First process";
        myButton().Content(box_value(firstProcessString));
        winrt::Windows::Foundation::Uri uri(c_uri);
        co_await winrt::Windows::System::Launcher::LaunchUriAsync(uri);
    }
}
