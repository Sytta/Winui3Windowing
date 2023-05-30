// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"
#include "MainWindow.xaml.h"

#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Windows.ApplicationModel.Activation.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace WebView2Sample;
using namespace WebView2Sample::implementation;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace MWAL = winrt::Microsoft::Windows::AppLifecycle;
namespace WAMA = winrt::Windows::ApplicationModel::Activation;

winrt::Microsoft::UI::Xaml::Window App::m_window{ nullptr };
/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
}

/// <summary>
/// Invoked when the application is launched.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const&)
{
    m_window = make<MainWindow>();
    CreateRootFrame(m_window);
    m_window.Activate();
}

void App::CreateRootFrame(Microsoft::UI::Xaml::Window const& window)
{
    namespace WUXC = Microsoft::UI::Xaml::Controls;

    WUXC::Frame rootFrame{ nullptr };
    auto content = window.Content();
    if (content)
    {
        rootFrame = content.try_as<WUXC::Frame>();
    }

    // Don't repeat app initialization when the Window already has content,
    // just ensure that the window is active    
    if (rootFrame == nullptr)
    {
        rootFrame = WUXC::Frame();
        window.Content(rootFrame);
    }

    if (rootFrame.Content() == nullptr)
    {
        bool navigationSuccess = rootFrame.Navigate(winrt::xaml_typename<MainPage>());
        if (!navigationSuccess)
        {
            winrt::terminate();
        }
    }
}