#include "pch.h"
#include "CoreApplicationView.h"
#if __has_include("CoreApplicationView.g.cpp")
#include "CoreApplicationView.g.cpp"
#endif

namespace winrt::CloseException::implementation
{
    int32_t CoreApplicationView::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void CoreApplicationView::MyProperty(int32_t /*value*/)
    {
        throw hresult_not_implemented();
    }
}
