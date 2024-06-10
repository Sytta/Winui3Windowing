// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once
#include <winrt/Microsoft.UI.Xaml.h>

#include "App.xaml.g.h"

namespace WF = winrt::Windows::Foundation;
namespace WUX = winrt::Microsoft::UI::Xaml;

namespace winrt::CloseException::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        void OnWindowClosed(Windows::Foundation::IInspectable const& /*sender*/, Microsoft::UI::Xaml::WindowEventArgs const& args);
        WF::IAsyncAction CreateViewOnNewThread();

        winrt::event_token Destroyed(WUX::RoutedEventHandler const& handler) { return m_destroyedEvent.add(handler); }
        void Destroyed(winrt::event_token const& token) { m_destroyedEvent.remove(token); }

        static thread_local WUX::Window m_window;
        winrt::event<WUX::RoutedEventHandler> m_destroyedEvent;
    };
}
