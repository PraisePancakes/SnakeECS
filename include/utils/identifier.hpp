#pragma once
#include "../common_types.hpp"

namespace snek
{
    namespace uuid
    {
        static u64 S_component_id = 0;

        static inline auto GenerateEntityID() noexcept
        {
            static u64 new_id = 0;
            u64 old = new_id;
            new_id++;
            return old;
        };

        template <typename Component>
        static inline auto GenerateComponentID() noexcept
        {
            static u64 id = S_component_id++;
            return id;
        };

    }

}