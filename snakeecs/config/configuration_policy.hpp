#pragma once
#include <iostream>
#include "../ecs/world_policy.hpp"

#define _SNEK_CONFIG true
#ifdef _SNEK_CONFIG

namespace config
{
    namespace test_config
    {
        class ComponentA
        {
        };

        class ComponentB
        {
        };

        using ComponentTypes = snek::component_list<ComponentA, ComponentB>;
        using configuration_policy = snek::world_policy<std::uint64_t, ComponentTypes>;

    }
}

#endif
