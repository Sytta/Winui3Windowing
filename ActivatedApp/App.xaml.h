// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"
#include "winrt/Microsoft.Windows.AppLifecycle.h"

namespace winrt::ActivatedApp::implementation
{
    struct App : AppT<App>
    {
        App();
        ~App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        void OnActivated(Microsoft::Windows::AppLifecycle::AppActivationArguments const&);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
