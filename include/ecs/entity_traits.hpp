#pragma once

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

        template <typename T, typename MaskType>
        struct has_cmp_mask
        {

            template <typename U>
            constexpr static std::true_type test(decltype(&T::GetComponentMask));

            template <typename U>
            constexpr static std::false_type test(...);

            using type = decltype(test<T>(0));
            constexpr static bool value = std::is_same_v<MaskType, strip_t<decltype(std::declval<T>().GetComponentMask())>>;
        };

        template <typename T, typename U>
        constexpr bool has_cmp_mask_v = has_cmp_mask<T, U>::value;

        template <typename T, typename TagType>
        struct has_tag
        {
            template <typename U>
            constexpr static std::true_type test(decltype(&T::GetTag));

            template <typename U>
            constexpr static std::false_type test(...);

            using type = decltype(test<T>(0));
            constexpr static bool value = std::is_same_v<TagType, strip_t<decltype(std::declval<T>().GetTag())>>;
        };

        template <typename T, typename U>
        constexpr bool has_tag_v = has_tag<T, U>::value;

    }

}
