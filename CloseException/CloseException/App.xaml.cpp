// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"
#include "MainWindow.xaml.h"

#include <ppltasks.h>
#include <pplawait.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace CloseException;
using namespace CloseException::implementation;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

thread_local WUX::Window winrt::CloseException::implementation::App::m_window = nullptr;

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
    m_window.Closed(winrt::auto_revoke, { get_weak(), &App::OnWindowClosed });
    m_window.Activate();
}

bool firstTime = true;

void App::OnWindowClosed(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::WindowEventArgs const& args)
{
    if (firstTime)
    {
        firstTime = false;
        args.Handled(true);

        m_window.DispatcherQueue().TryEnqueue([=]()
            {
                m_window.Close();
            });
    }
}

Windows::Foundation::IAsyncAction App::CreateViewOnNewThread()
{
    auto strongThis{ get_strong() };

    // Create a signal to notify once the Window has been created in the background thread
    auto windowCreationEvent = concurrency::task_completion_event<void>();
    auto task = concurrency::create_task(windowCreationEvent);

    std::thread newThread = std::thread([=]()
        {
            winrt::init_apartment(winrt::apartment_type::single_threaded);

            WUX::Application::Start([=](auto&&)
                {
                    windowCreationEvent.set();
                });

            m_destroyedEvent(*this, nullptr);
        });

    // NOTE: Detach thread since WUX::Application::Start() will not return until the window closes
    newThread.detach();

    // Wait for Window creation on background before proceeding
    co_await task;
}
