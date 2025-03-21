#pragma once
#include <cstdint>
#include <limits>
namespace snek
{
    namespace traits
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

        template <>
        struct tombstone_t<int>
        {
            using value_type = int;
            static constexpr int null_v = std::numeric_limits<int>::max();
        };
    }
}