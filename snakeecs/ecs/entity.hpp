#pragma once
#include <limits>
#include <cstdint>
#include <iostream>

namespace snek::entity
{
    template <typename T>
    struct entity_traits
    {
    };

    template <>
    struct entity_traits<std::uint64_t>
    {
        using value_type = std::uint64_t;
    };

    template <>
    struct entity_traits<std::uint32_t>
    {
        using value_type = std::uint32_t;
    };

    template <typename T, typename = void>
    struct is_entity_type
    {
        static constexpr bool value = false;
    };
    template <typename T>
    struct is_entity_type<T, std::void_t<typename entity_traits<T>::value_type>>
    {
        static constexpr bool value = true;
    };

};