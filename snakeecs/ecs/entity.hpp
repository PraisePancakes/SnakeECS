#pragma once
#include <limits>
#include <cstdint>
#include <iostream>

namespace snek::entity
{
    template <typename T>
    struct entity_traits;

    template <>
    struct entity_traits<std::uint32_t>
    {
        using entity_type = std::uint32_t; // higher 32 bits
        using version_type = std::uint8_t; // lower 8 bits
        constexpr static version_type version_mask = 0xFF;
    };

    template <>
    struct entity_traits<std::uint64_t>
    {
        using entity_type = std::uint64_t; // higher 64 bits
        using version_type = std::uint8_t; // lower 8 bits
        constexpr static version_type version_mask = 0xFF;
    };

    template <typename T, typename = void>
    struct is_entity_type : std::false_type
    {
    };
    template <typename T>
    struct is_entity_type<T, std::void_t<typename entity_traits<T>::entity_type>> : std::true_type
    {
    };

};