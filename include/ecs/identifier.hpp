
#pragma once
#include "../common_types.hpp"
#include "interface.hpp"

namespace snek
{
    namespace core
    {
        template <typename SizeT = u64>
        inline auto GenerateEntityID() noexcept
        {
            static SizeT new_id = 0;
            SizeT old = new_id;
            new_id++;
            return old;
        };

        template <typename Component, typename = std::enable_if_t<std::is_base_of_v<BaseComponent, Component>>>
        size_t GenerateComponentTypeID()
        {
            return typeid(Component).hash_code();
        };

    }
}