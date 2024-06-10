#pragma once

#include "CoreApplication.g.h"

namespace winrt::CloseException::implementation
{
    static SharedServices::CoreApplication m_instance{ nullptr };

    struct CoreApplication : CoreApplicationT<CoreApplication>
    {
        CoreApplication();

        /// <summary>
        /// It returns a singleton instance of CoreApplication.
        /// </summary>
        static SharedServices::CoreApplication Get();

        /// <summary>
        /// This event is raised when all views have been closed
        /// </summary>
        winrt::event_token ViewsClosed(WUX::RoutedEventHandler const& handler) { return m_viewsClosedEvent.add(handler); }
        void ViewsClosed(winrt::event_token const& token) { m_viewsClosedEvent.remove(token); }

    private:
        SharedServices::CoreApplicationView m_mainView{ nullptr };
        winrt::event<WUX::RoutedEventHandler> m_viewsClosedEvent;
    };
}

namespace winrt::CloseException::factory_implementation
{
    struct CoreApplication : CoreApplicationT<CoreApplication, implementation::CoreApplication>
    {
    };
}
