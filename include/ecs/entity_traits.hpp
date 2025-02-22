#pragma once
#include <type_traits>
//  sfinae checks for entity traits
namespace snek
{
    namespace internal
    {
        template <typename E>
        struct entt_traits
        {
            using tag_t = E::tag_t;
            using mask_t = E::mask_t;
            using size_type = E::size_type;
        };

        template <typename T>
        struct strip
        {
            using type = std::remove_reference_t<std::remove_cv_t<T>>;
        };

        template <typename T>
        using strip_t = typename strip<T>::type;

        template <typename T, typename MaskType, typename = void>
        struct has_cmp_mask : std::false_type
        {
            using mask_type = void;
        };

        template <typename T, typename MaskType>
        struct has_cmp_mask<T, MaskType, std::void_t<decltype(T::GetComponentMask)>> : std::true_type
        {
            using mask_type = decltype(std::declval<T>().GetComponentMask());
            static_assert(sizeof(MaskType) == sizeof(strip_t<mask_type>));
        };

        template <typename T, typename U>
        constexpr static bool has_cmp_mask_v = has_cmp_mask<T, U>::value;

        template <typename T, typename TagType, typename = void>
        struct has_tag : std::false_type
        {
            using tag_type = void;
        };

        template <typename T, typename TagType>
        struct has_tag<T, TagType, std::void_t<decltype(T::GetTag)>> : std::true_type
        {
            using tag_type = decltype(std::declval<T>().GetTag());
            static_assert(sizeof(TagType) == sizeof(strip_t<tag_type>));
        };

        template <typename T, typename U>
        constexpr bool has_tag_v = has_tag<T, U>::value;

        template <typename Entity, typename TagType, typename MaskType>
        struct has_entt_traits
        {
            constexpr static bool value = (has_tag<Entity, TagType>::value && has_cmp_mask<Entity, MaskType>::value);
        };

    }

}
