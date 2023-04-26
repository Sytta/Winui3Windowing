// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainPage.g.h"

namespace winrt::WebView2Sample::implementation
{
    namespace MUX = winrt::Microsoft::UI::Xaml;
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        void OnLoaded(IInspectable const&, IInspectable const&);
        void OnUnloaded(IInspectable const&, IInspectable const&);

        void OnPreviewKeyUpEvent(IInspectable const&, MUX::Input::KeyRoutedEventArgs const& args);
        void OnPreviewKeyDownEvent(IInspectable const&, MUX::Input::KeyRoutedEventArgs const& args);

        MUX::UIElement::PreviewKeyUp_revoker m_previewKeyUpRevoker;
        MUX::UIElement::PreviewKeyDown_revoker m_previewKeyDownRevoker;
    };
}

namespace winrt::WebView2Sample::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
