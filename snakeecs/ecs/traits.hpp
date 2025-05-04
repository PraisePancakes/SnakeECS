#pragma once
#include <cstdint>
#include <limits>
namespace snek::traits
{

    template <typename T>
    struct tombstone_t;

    // nullable integral types
    template <>
    struct tombstone_t<std::uint32_t>
    {
        using value_type = std::uint32_t;
        static constexpr std::uint32_t null_v = std::numeric_limits<std::uint32_t>::max();
    };

    template <>
    struct tombstone_t<std::uint64_t>
    {
        using value_type = std::uint64_t;
        static constexpr std::uint64_t null_v = std::numeric_limits<std::uint64_t>::max();
    };

    template <>
    struct tombstone_t<int>
    {
        using value_type = int;
        static constexpr int null_v = std::numeric_limits<int>::max();
    };

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
}
