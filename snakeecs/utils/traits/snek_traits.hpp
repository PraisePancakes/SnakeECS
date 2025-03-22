#pragma once
#include <iostream>

namespace snek
{
    namespace traits
    {
        template <bool V, typename T1, typename T2>
        struct conditional
        {
        };

        template <typename T1, typename T2>
        struct conditional<true, T1, T2>
        {
            using type = T1;
        };

        template <typename T1, typename T2>
        struct conditional<false, T1, T2>
        {
            using type = T2;
        };

        template <bool V, typename T1, typename T2>
        using conditional_t = typename conditional<V, T1, T2>::type;

        template <typename T, typename = void>
        struct has_value_type : std::false_type
        {
        };

        template <typename T>
        struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type
        {
        };

        template <typename T>
        struct has_allocate
        {
        };

        template <typename T>
        struct type_is_allocator
        {
        };

    }
};