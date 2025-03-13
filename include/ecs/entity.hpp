#pragma once
#include <type_traits>
#include "../utils/traits.hpp"
#include <cstdint>
//  sfinae checks for entity traits
namespace snek
{

    template <typename T, typename = void>
    struct entity_traits;

    template <typename T>
    struct entity_traits<T, std::enable_if_t<std::is_class_v<T>>>
    {
        using value_type = std::underlying_type_t<T>;
    };

    template <typename T>
    struct entity_traits<T, std::enable_if_t<std::is_enum_v<T>>>
    {
        using value_type = std::underlying_type_t<T>;
    };

    template <>
    struct entity_traits<std::uint32_t>
    {
        using value_type = std::uint32_t;
        using entity_type = std::uint32_t;
    };

    template <>
    struct entity_traits<std::uint64_t>
    {
        using value_type = std::uint64_t;
        using entity_type = std::uint64_t;
    };

    template <typename T, typename MaskType, typename = void>
    struct has_cmp_mask : std::false_type
    {
        using mask_type = void;
    };

    template <typename T, typename MaskType>
    struct has_cmp_mask<T, MaskType, std::void_t<decltype(T::GetComponentMask)>> : std::true_type
    {
        using mask_type = decltype(std::declval<T>().GetComponentMask());
        static_assert(sizeof(MaskType) == sizeof(utils::strip_t<mask_type>));
    };

    template <typename T, typename U>
    constexpr static bool has_cmp_mask_v = has_cmp_mask<T, U>::value;

    template <typename Entity, typename MaskType>
    struct has_snek_entity_traits
    {
        constexpr static bool value = (has_cmp_mask<Entity, MaskType>::value);
    };

}
