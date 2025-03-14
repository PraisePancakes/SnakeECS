#pragma once

namespace snek
{
    namespace utils
    {
        /** DEBUG ONLY */
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
    }
}