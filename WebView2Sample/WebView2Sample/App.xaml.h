// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"

namespace winrt::WebView2Sample::implementation
{
    struct App : AppT<App>
    {
        App();
        static winrt::Microsoft::UI::Xaml::Window Window() { return m_window; };

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        void CreateRootFrame(Microsoft::UI::Xaml::Window const& window);

    private:
        static winrt::Microsoft::UI::Xaml::Window m_window;
    };
}
