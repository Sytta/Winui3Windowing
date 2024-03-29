// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "CustomControl.h"
#if __has_include("CustomControl.g.cpp")
#include "CustomControl.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Winui3UI::implementation
{
    CustomControl::CustomControl()
    {
        DefaultStyleKey(winrt::box_value(L"Winui3UI.CustomControl"));
    }

    int32_t CustomControl::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void CustomControl::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}
