// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"
#include <winrt/Microsoft.Windows.AppLifecycle.h>

namespace MWAL = winrt::Microsoft::Windows::AppLifecycle;

namespace winrt::OpenWith::implementation
{

    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        void OnRedirectionActivated(MWAL::AppActivationArguments const& e);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
