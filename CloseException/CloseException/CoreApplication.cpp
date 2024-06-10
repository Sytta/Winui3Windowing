#include "pch.h"
#include "CoreApplication.h"
#if __has_include("CoreApplication.g.cpp")
#include "CoreApplication.g.cpp"
#endif

namespace winrt::CloseException::implementation
{

    SharedServices::CoreApplication CoreApplication::Get()
    {
        if (m_instance == nullptr)
        {
            m_instance = winrt::make<implementation::CoreApplication>();
        }

        return m_instance;
    }
}
