#pragma once

namespace snek
{
    namespace utils
    {
        /** DEBUG ONLY */
        template <typename C, typename X, typename T>
        struct replace_type
        {
            using type = T;
        };

        template <typename S, typename T>
        struct replace_type<S, S, T>
        {
            using type = T;
        };

        template <typename S, typename T>
        struct replace_type<S *, S, T>
        {
            using type = T *;
        };

        template <typename S, size_t N, typename T>
        struct replace_type<S *[N], S, T>
        {
            using type = T *[N];
        };

        template <typename R, typename S, typename... Args, typename T>
        struct replace_type<R (*)(Args...), S, T>
        {
            using ret_type = typename replace_type<R, S, T>::type;
            using type = ret_type (*)(typename replace_type<Args, S, T>::type...);
        };

    }
}