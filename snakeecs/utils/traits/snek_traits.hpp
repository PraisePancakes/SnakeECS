#pragma once
#include <iostream>

namespace snek
{
    namespace traits
    {
        template <bool V, typename T1, typename T2>
        struct conditional;

        template <typename T1, typename T2>
        struct conditional<true, T1, T2>
        {
            using type = T1;
            constexpr static bool value = true;
        };

        template <typename T1, typename T2>
        struct conditional<false, T1, T2>
        {
            using type = T2;
            constexpr static bool value = false;
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
        constexpr static bool has_value_type_v = has_value_type<T>::value;

        template <typename T, typename = void>
        struct has_allocate : std::false_type
        {
        };

        template <typename T>
        struct has_allocate<T, std::void_t<decltype(std::declval<T>().allocate(std::declval<size_t>()))>> : std::true_type
        {
        };

        template <typename T, typename = void>
        struct has_deallocate : std::false_type
        {
        };

        template <typename T>
        struct has_deallocate<T,
                              std::void_t<decltype(std::declval<T>().deallocate(std::declval<typename T::value_type *>(), std::declval<size_t>()))>> : std::true_type
        {
        };

        template <typename T, typename = void>
        struct type_is_allocator : std::false_type
        {
        };

        template <typename T>
        struct type_is_allocator<T,
                                 std::void_t<std::enable_if_t<has_allocate<T>::value && has_value_type_v<T> && has_deallocate<T>::value>>>
            : std::true_type
        {
        };

        template <typename T, typename = void>
        struct tlist_has_size_constant : std::false_type
        {
        };

        template <typename T>
        struct tlist_has_size_constant<T, std::void_t<decltype(std::declval<T>().size)>> : std::true_type
        {
        };

        template <typename T, typename = void>
        struct tlist_has_list_size_constant : std::false_type
        {
        };

        template <typename T>
        struct tlist_has_list_size_constant<T, std::void_t<decltype(std::declval<T>().list_size)>> : std::true_type
        {
        };

        template <typename, typename = void>
        struct is_type_list : std::false_type
        {
        };

        template <template <typename...> class T, typename... Ts>
        struct is_type_list<T<Ts...>,
                            std::enable_if_t<tlist_has_size_constant<T<Ts...>>::value && tlist_has_list_size_constant<T<Ts...>>::value>> : std::true_type
        {
        };

    }
};