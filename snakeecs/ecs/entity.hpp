#pragma once
#include <limits>
#include <cstdint>
#include <iostream>

namespace snek
{
    template <typename T>
    struct tombstone_t;

    // nullable entities
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

    // nullable components
    template <typename T>
    struct tombstone_t<T *>
    {

        using value_type = T;
        static constexpr T *null_v = nullptr;
    };

    namespace entity
    {

        template <typename T>
        struct entity_traits;

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
        struct is_entity_type : std::false_type
        {
        };

        template <typename T>
        struct is_entity_type<T, std::void_t<typename entity_traits<T>::value_type>> : std::true_type
        {
        };

    }
}