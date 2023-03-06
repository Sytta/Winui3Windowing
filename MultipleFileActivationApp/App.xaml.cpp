// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"
#include "MainWindow.xaml.h"

#include <wil/resource.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/microsoft.ui.interop.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>

using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Navigation;
using namespace winrt::MultipleFileActivationApp;
using namespace winrt::MultipleFileActivationApp::implementation;

namespace MWAL = winrt::Microsoft::Windows::AppLifecycle;
namespace WAMA = winrt::Windows::ApplicationModel::Activation;
namespace WFC = winrt::Windows::Foundation::Collections;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

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
    window = make<MainWindow>();
    window.Activate();
}

// The code below is taken from the sample app: https://github.com/microsoft/WindowsAppSDK-Samples/blob/main/Samples/AppLifecycle/Instancing/cpp-winui-packaged

void OnActivated(const IInspectable&, const MWAL::AppActivationArguments& /*args*/)
{
    // Debug point
}


winrt::event_token activationToken{};
wil::unique_event redirectEventHandle{};

winrt::fire_and_forget Redirect(MWAL::AppInstance keyInstance, MWAL::AppActivationArguments args)
{
    // Using this type of event ensures that it gets signaled when it 
    // goes out of scope, even if the RedirectActivationToAsync fails.
    wil::event_set_scope_exit ensure_signaled =
        wil::SetEvent_scope_exit(redirectEventHandle.get());
    co_await keyInstance.RedirectActivationToAsync(args);
}

// Replaces the WinMain generated in App.xaml.g.hpp, which is suppressed
// by defining DISABLE_XAML_GENERATED_MAIN in the project properties.
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    // WWINMAIN IS CALLED MULTIPLE TIMES IN CASE OF MULTI FILE ACTIVATION
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    bool isRedirect = false;

    // Find out what kind of activation this is.
    MWAL::AppActivationArguments args = MWAL::AppInstance::GetCurrent().GetActivatedEventArgs();
    MWAL::ExtendedActivationKind kind = args.Kind();

    if (kind == MWAL::ExtendedActivationKind::Launch)
    {
        // Only allow 1 launch instance to be created
        MWAL::AppInstance keyInstance = MWAL::AppInstance::FindOrRegisterForKey(L"testString");

        // If we successfully registered the file name, we must be the
        // only instance running that was activated for this file.
        if (keyInstance.IsCurrent())
        {
            // Report successful file name key registration.
            activationToken = keyInstance.Activated([&keyInstance](
                const auto& sender, const MWAL::AppActivationArguments& args)
                { OnActivated(sender, args); }
            );
        }
        else
        {
            // We're in an STA so we must not block the thread by
            // waiting on the async call. Instead, we'll move the call
            // to a separate thread, and use an event to synchronize.
            isRedirect = true;
            redirectEventHandle.create();
            Redirect(keyInstance, args);
            DWORD handleIndex = 0;
            HANDLE rawHandle = redirectEventHandle.get();
            if (CoWaitForMultipleObjects(CWMO_DEFAULT, INFINITE, 1, &rawHandle, &handleIndex) != 0)
            {
                OutputDebugString(L"Error waiting on event");
            }
        }
    }
    else if (kind == MWAL::ExtendedActivationKind::File)
    {
        try
        {
            // This is a file activation. Allow creation of new process.
            WAMA::IFileActivatedEventArgs fileArgs = args.Data().as<WAMA::IFileActivatedEventArgs>();

            if (fileArgs)
            {
                auto files = fileArgs.Files();
                // SIZE = number of files chosen
                uint32_t size = files.Size();
            }
        }
        catch (...)
        {
            OutputDebugString(L"Error getting instance information");
        }
    }

    if (!isRedirect)
    {
        ::winrt::Microsoft::UI::Xaml::Application::Start(
            [](auto&&)
            {
                ::winrt::make<::winrt::MultipleFileActivationApp::implementation::App>();
            });
    }

    return 0;
}