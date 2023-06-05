// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "winrt/Microsoft.UI.Xaml.h"
#include "winrt/Microsoft.UI.Xaml.Markup.h"
#include "winrt/Microsoft.UI.Xaml.Controls.Primitives.h"
#include "CustomControl.g.h"

namespace winrt::Winui3UI::implementation
{
    struct CustomControl : CustomControlT<CustomControl>
    {
        CustomControl();

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::Winui3UI::factory_implementation
{
    struct CustomControl : CustomControlT<CustomControl, implementation::CustomControl>
    {
    };
}
