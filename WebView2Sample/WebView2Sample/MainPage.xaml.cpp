// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainPage.xaml.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif

#include "App.xaml.h"

#include <format>
#include <sstream>
#include <string>

#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Input.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::WebView2Sample::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }

    void MainPage::OnLoaded(IInspectable const&, IInspectable const&)
    {
        MUX::Controls::Frame frame = App::Window().Content().try_as<MUX::Controls::Frame>();
        if (frame != nullptr)
        {
            if (!m_previewKeyUpRevoker)
            {
                m_previewKeyUpRevoker = frame.PreviewKeyUp(auto_revoke, { get_weak(), &MainPage::OnPreviewKeyUpEvent });
            }

            if (!m_previewKeyDownRevoker)
            {
                m_previewKeyDownRevoker = frame.PreviewKeyDown(auto_revoke, { get_weak(), &MainPage::OnPreviewKeyDownEvent });
            }
        }
    }

    void MainPage::OnUnloaded(IInspectable const&, IInspectable const&)
    {
        m_previewKeyUpRevoker = {};
        m_previewKeyDownRevoker = {};
    }

    void MainPage::OnPreviewKeyUpEvent(IInspectable const&, MUX::Input::KeyRoutedEventArgs const& args)
    {
        auto vKey = args.Key();
        OutputDebugStringW(L"OnPreviewKeyUpEvent\n");
    }

    void MainPage::OnPreviewKeyDownEvent(IInspectable const&, MUX::Input::KeyRoutedEventArgs const& args)
    {
        auto vKey = args.Key();
        OutputDebugStringW(L"OnPreviewKeyDownEvent\n");
    }

    void MainPage::MyWebView_KeyDown(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args)
    {
        auto vKey = args.Key();
        OutputDebugStringW(L"MyWebView_KeyDown\n");
    }
}



