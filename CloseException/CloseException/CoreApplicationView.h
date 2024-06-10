#pragma once

#include "CoreApplicationView.g.h"

namespace winrt::CloseException::implementation
{
    struct CoreApplicationView : CoreApplicationViewT<CoreApplicationView>
    {
        CoreApplicationView() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::CloseException::factory_implementation
{
    struct CoreApplicationView : CoreApplicationViewT<CoreApplicationView, implementation::CoreApplicationView>
    {
    };
}
