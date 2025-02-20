#pragma once
#include <type_traits>
//  sfinae checks for entity traits
namespace snek
{
    namespace internal
    {
        template <typename T>
        struct strip
        {
            using type = std::remove_pointer<typename std::decay<T>::type>::type;
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
            using mask_type = std::conditional_t<sizeof(strip_t<MaskType>) == sizeof(typename T::mask_t), MaskType, void>;
        };

        template <typename T, typename U>
        constexpr bool has_cmp_mask_v = has_cmp_mask<T, U>::value;

        template <typename T, typename TagType, typename = void>
        struct has_tag : std::false_type
        {
            using tag_type = void;
        };

        template <typename T, typename TagType>
        struct has_tag<T, TagType, std::void_t<decltype(T::GetTag)>> : std::true_type
        {
            using tag_type = std::conditional_t<sizeof(strip_t<TagType>) == sizeof(typename T::tag_t), TagType, void>;
        };

        template <typename T, typename U>
        constexpr bool has_tag_v = has_tag<T, U>::value;

        template <typename T>
        struct has_entity_traits
        {
            static constexpr bool value = (has_tag<T, typename T::tag_t>::value && has_cmp_mask<T, typename T::mask_t>::value);
        };

        template <typename E>
        struct ent_traits
        {
            using tag_t = E::tag_t;
            using mask_t = E::mask_t;
            using size_type = E::size_type;
        };

    }

}
