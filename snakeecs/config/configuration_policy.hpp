#pragma once
#include <iostream>
#include "../ecs/world_policy.hpp"

// set config to false in release
#define _SNEK_CONFIG true
#ifdef _SNEK_CONFIG

namespace snek
{

    namespace test_config
    {
        class component_a
        {
        };

        class component_b
        {
        };

        using component_types = snek::component_list<component_a, component_b>;
        // config policy
        //@params
        // #ENTITY_TYPE
        // #COMPONENT_TYPE
        // #ALLOCATOR_TYPE
        using configuration_policy = snek::world_policy<std::uint64_t, component_types, int>;

    }

}

#endif
