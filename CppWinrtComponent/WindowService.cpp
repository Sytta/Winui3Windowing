#include "pch.h"
#include "WindowService.h"
#if __has_include("WindowService.g.cpp")
#include "WindowService.g.cpp"
#endif

#include <microsoft.ui.xaml.window.h>
#include <winrt/microsoft.ui.interop.h>

namespace winrt::CppWinrtComponent::implementation
{
	namespace MUX = winrt::Microsoft::UI::Xaml;
	namespace MU = winrt::Microsoft::UI;
	namespace MUW = winrt::Microsoft::UI::Windowing;
	namespace WF = winrt::Windows::Foundation;

	HWND GetWindowHandle(MUX::Window const& window)
	{
		try
		{
			auto windowNative{ window.try_as<::IWindowNative>() };
			winrt::check_bool(windowNative);

			HWND hWnd{ 0 };
			windowNative->get_WindowHandle(&hWnd);

			return hWnd;
		}
		catch (winrt::hresult_error const&)
		{
			return { 0 };
		}
	}

	/*static*/ MUX::Window WindowService::GetCurrentWindow()
	{
		WINRT_ASSERT(s_currentWindow != nullptr && "[WindowService] GetCurrentWindow: Current window should not be nullptr. This method should be called from a UI thread");
		return s_currentWindow;
	}

	/*static*/ uint64_t WindowService::GetCurrentAppWindowId()
	{
		WINRT_ASSERT(s_currentAppWindow != nullptr && "[WindowService] GetCurrentAppWindowId: Current AppWindow should not be nullptr. This method should be called from a UI thread");
		if (s_currentAppWindow == nullptr)
		{
			return 0;
		}

		return s_currentAppWindow.Id().Value;
	}

	thread_local MUX::Window WindowService::s_currentWindow{ nullptr };
	thread_local MUW::AppWindow WindowService::s_currentAppWindow{ nullptr };
	thread_local winrt::event_token WindowService::s_threadWindowClosedToken{};

	/*static*/ void WindowService::SetCurrentWindow(MUX::Window const& window)
	{
		WINRT_ASSERT(s_currentWindow == nullptr && "[WindowService] SetCurrentWindow: Current window should be nullptr");
		if (s_currentWindow == nullptr)
		{
			s_currentWindow = window;
			s_threadWindowClosedToken = s_currentWindow.Closed(&WindowService::OnWindowClosed);

			// Set windowing variables that will be requested often
			HWND handle = GetWindowHandle(s_currentWindow);
			MU::WindowId appWindowId = MU::GetWindowIdFromWindow(handle);
			s_currentAppWindow = MUW::AppWindow::GetFromWindowId(appWindowId);
			WINRT_ASSERT(s_currentAppWindow != nullptr && "[WindowService] SetCurrentWindow: Current AppWindow should not be nullptr.");
		}
	}

	/*static*/ MUW::AppWindow WindowService::GetCurrentAppWindow()
	{
		WINRT_ASSERT(s_currentAppWindow != nullptr && "[WindowService] GetCurrentAppWindow: Current AppWindow should not be nullptr. This method should be called from a UI thread");
		return s_currentAppWindow;
	}

	/*static*/ void WindowService::OnWindowClosed(WF::IInspectable const&, MUX::WindowEventArgs const&)
	{
		WINRT_ASSERT(s_threadWindowClosedToken);
		s_currentWindow.Closed(s_threadWindowClosedToken);
		s_threadWindowClosedToken = {};

		s_currentWindow = nullptr;
		s_currentAppWindow = nullptr;
	}
}
