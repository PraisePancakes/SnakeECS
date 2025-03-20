#pragma once
#include <cstdint>

namespace snek
{
    namespace utils
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

        /** DEBUG */
        template <typename T>
        struct name_of
        {
            static constexpr auto value = "";
        };

        template <>
        struct name_of<int>
        {
            static constexpr auto value = "int";
        };

        template <>
        struct name_of<char>
        {
            static constexpr auto value = "char";
        };

        template <>
        struct name_of<float>
        {
            static constexpr auto value = "float";
        };

        template <>
        struct name_of<long>
        {
            static constexpr auto value = "long";
        };

        template <>
        struct name_of<void>
        {
            static constexpr auto value = "void";
        };

        template <>
        struct name_of<std::uint64_t>
        {
            static constexpr auto value = "uint64_t";
        };

        template <>
        struct name_of<std::uint32_t>
        {
            static constexpr auto value = "uint32_t";
        };

        template <typename T>
        struct type_descriptor
        {

            std::string stringify()
            {
                if constexpr (std::is_const_v<T>)
                {
                    return "const " + type_descriptor<std::remove_const_t<T>>().stringify();
                }
                if constexpr (std::is_pointer_v<T>)
                {
                    return type_descriptor<std::remove_pointer_t<T>>().stringify() + "*";
                }
                if constexpr (std::is_reference_v<T>)
                {
                    return type_descriptor<std::remove_reference_t<T>>().stringify() + "&";
                }

                return name_of<T>::value;
            };
        };

        template <typename... Cs>
        struct type_list;

        namespace internal
        {
            template <std::int64_t N, typename C, typename... U>
            struct index_of;

            template <std::int64_t N, typename C, typename U>
            struct index_of<N, C, U>
            {
                constexpr static int64_t value = (std::is_same_v<C, U>) ? N : -1;
            };

            template <int64_t N, typename T, typename C, typename... Ts>
            struct index_of<N, T, type_list<C, Ts...>>
            {
                constexpr static int64_t value = std::is_same_v<T, C> ? N : index_of<N + 1, T, type_list<Ts...>>::value;
            };

        }

        template <typename T, typename TypeList>
        constexpr auto index_of()
        {
            return internal::index_of<0, T, TypeList>::value;
        }

    }

   
}