// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"
#include "MainWindow.xaml.h"

#include <wil/resource.h>
#include <winrt/Windows.ApplicationModel.Activation.h>

using namespace winrt::OpenWith::implementation;

namespace MUX = winrt::Microsoft::UI::Xaml;
namespace WF = winrt::Windows::Foundation;
namespace WAMA = winrt::Windows::ApplicationModel::Activation;

wil::unique_event redirectEventHandle;
winrt::event_token activationToken;

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
    UnhandledException([this](IInspectable const&, MUX::UnhandledExceptionEventArgs const& e)
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
void App::OnLaunched(MUX::LaunchActivatedEventArgs const&)
{
    window = make<MainWindow>();
    window.Activate();
}

void App::OnRedirectionActivated(MWAL::AppActivationArguments const& e)
{
    // Throws RPC not avaiable exception here
    [[maybe_unused]] WAMA::IActivatedEventArgs activatedArgs = e.Data().try_as<WAMA::IActivatedEventArgs>();

    auto newWindow = make<MainWindow>();
    newWindow.Activate();
}

///////////////////////////////////////////////////////////////////////////////
// WinMain ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

winrt::fire_and_forget Redirect(MWAL::AppInstance keyInstance, MWAL::AppActivationArguments args)
{
    // Using this type of event ensures that it gets signaled when it 
    // goes out of scope, even if the RedirectActivationToAsync fails.
    wil::event_set_scope_exit ensure_signaled =
        wil::SetEvent_scope_exit(redirectEventHandle.get());
    co_await keyInstance.RedirectActivationToAsync(args);
}

static void KeyInstance_Activated(WF::IInspectable const& /*sender*/, MWAL::AppActivationArguments const& args)
{
    auto currentApplication = MUX::Application::Current();
    if (currentApplication != nullptr)
    {
        auto currentApp = currentApplication.try_as<App>();
        WINRT_ASSERT(currentApp != nullptr && L"WUX::Application::Current cannot be casted to App class");
        if (currentApp != nullptr)
        {
            currentApp.get()->OnRedirectionActivated(args);
        }
    }
}

static bool DecideRedirection()
{
    bool isRedirect = false;

    try
    {
        MWAL::AppInstance keyInstance = MWAL::AppInstance::FindOrRegisterForKey(L"uniqueString");

        // If we successfully registered the key, we must be the only instance running that was activated. 
        if (keyInstance.IsCurrent())
        {
            // Hook up the Activated event, to allow for this instance of the app
            // getting reactivated as a result of multi-instance redirection.
            activationToken = keyInstance.Activated(&KeyInstance_Activated);
        }
        else
        {
            isRedirect = true;
            // We are not the first active instance of the app activated through normal launch, 
            // terminates the thread and redirect the activation arguments to the already active instance of the app.
            try
            {
                // BUG: Need to have a sign of when the ActivatedEventArgs comes from Open With and when it is not.
                // Sometimes (mostly during redirections), this call doesn't throw and only throws after redirection has already been
                // done, and we have no way to retrieve the arguments by then.
                // https://microsoft.visualstudio.com/OS/_backlogs/backlog/Windows%20Photos/Features%20and%20Scenarios/?workitem=45393066
                redirectEventHandle.create();
                MWAL::AppActivationArguments args = MWAL::AppInstance::GetCurrent().GetActivatedEventArgs();
                winrt::hstring kind = L"ActivationKind = " + winrt::to_hstring((int)args.Kind());
                OutputDebugStringW(kind.c_str());
                Redirect(keyInstance, args);

                DWORD handleIndex = 0;
                HANDLE rawHandle = redirectEventHandle.get();
                if (CoWaitForMultipleObjects(CWMO_DEFAULT, INFINITE, 1, &rawHandle, &handleIndex) != 0)
                {
                    OutputDebugStringW(L"Error waiting on event");
                }
            }
            catch (...)
            {
                OutputDebugStringW(L"[OpenWith] Exception catched for GetActivatedEventArgs()");
            }
        }
    }
    catch (...)
    {
        OutputDebugStringW(L"Exception happened in DecideRedirection");
    }

    return isRedirect;
}

// Replaces the WinMain generated in App.xaml.g.hpp, which is suppressed
// by defining DISABLE_XAML_GENERATED_MAIN in the project properties.
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    bool isRedirect = DecideRedirection();
    if (!isRedirect)
    {
        MUX::Application::Start([](auto&&) { ::winrt::make<App>(); });
    }

    return 0;
}