#pragma once
#include <cstdint>

namespace snek
{
    namespace utils
    {

        template <typename... Cs>
        struct type_list
        {
            static constexpr std::uint64_t size = sizeof...(Cs);
            static constexpr std::uint64_t list_size = 64;
        };

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