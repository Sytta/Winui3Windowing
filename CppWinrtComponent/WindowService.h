#pragma once

#include "WindowService.g.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Windowing.h>

namespace winrt::CppWinrtComponent::implementation
{
	struct WindowService
	{
		WindowService() = delete;
		static winrt::Microsoft::UI::Xaml::Window GetCurrentWindow();
		static uint64_t GetCurrentAppWindowId();

		static void SetCurrentWindow(winrt::Microsoft::UI::Xaml::Window const& window);
		static winrt::Microsoft::UI::Windowing::AppWindow GetCurrentAppWindow();

	private:
		static WindowService m_instance;

		static void OnWindowClosed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args);

		static thread_local winrt::Microsoft::UI::Xaml::Window s_currentWindow;
		static thread_local winrt::Microsoft::UI::Windowing::AppWindow s_currentAppWindow;
		static thread_local winrt::event_token s_threadWindowClosedToken;
	};
}

namespace winrt::CppWinrtComponent::factory_implementation
{
    struct WindowService : WindowServiceT<WindowService, implementation::WindowService>
    {
    };
}
