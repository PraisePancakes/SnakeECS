#pragma once
#include <type_traits>
namespace snek
{
    namespace utils
    {
        template <typename T>
        struct strip
        {
            using type = std::remove_reference_t<std::remove_cv_t<T>>;
        };

        template <typename T>
        using strip_t = typename strip<T>::type;
    }
}
