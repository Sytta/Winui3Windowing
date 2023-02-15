// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "Page2.g.h"

namespace winrt::CppWinrtComponent::implementation
{
    struct Page2 : Page2T<Page2>
    {
        Page2();

        void myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::CppWinrtComponent::factory_implementation
{
    struct Page2 : Page2T<Page2, implementation::Page2>
    {
    };
}
