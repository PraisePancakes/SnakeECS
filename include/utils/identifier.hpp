#pragma once
#include "../common_types.hpp"
#include "../ecs/component.hpp"

namespace snek
{
    class Component;

    namespace uuid
    {
        template <typename SizeT = u64>
        inline auto GenerateEntityID() noexcept
        {
            static SizeT new_id = 0;
            SizeT old = new_id;
            new_id++;
            return old;
        };

        template <typename C, typename = std::enable_if_t<std::is_base_of_v<Component, C>>>
        size_t GenerateComponentHashCode()
        {
            return typeid(C).hash_code();
        };
    }

}