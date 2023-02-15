// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "Page2.xaml.h"
#if __has_include("Page2.g.cpp")
#include "Page2.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::CppWinrtComponent::implementation
{
    Page2::Page2()
    {
        InitializeComponent();
    }

    void Page2::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));

        wchar_t textBuffer[256];
        const wchar_t* fmt = L"Thread: %lu, WindowId: %u";
        swprintf_s(textBuffer, fmt, GetCurrentThreadId(), WindowService::GetCurrentAppWindowId());

        Page2TextBlock().Text(textBuffer);
    }
}
