#pragma once
#include <iostream>
#include "../ecs/world_policy.hpp"
#include "../debug/debug.hpp"

// set config to false in release
#if _SNEK_CONFIG

namespace snek::test_config
{

    struct component_a
    {
        int x = 4;
        component_a(int x) : x(x) {};
        component_a() {};
    };

    struct component_b
    {
        int x = 5;

        component_b(int x) : x(x) {};
        };

    struct component_c
    {
        int a;
        char b;
        std::string c;
        component_c(int a, char b, std::string c) : a(a), b(b), c(c) {};
    };

    struct component_d
    {
    };

    using component_types = snek::component_list<component_a, component_b, component_c, component_d>;
    // config policy
    //@params
    // #ENTITY_TYPE
    // #COMPONENT_TYPE
    // #ALLOCATOR_TYPE
    using configuration_policy = snek::world_policy<std::uint64_t, component_types, std::allocator<std::uint64_t>>;

};

#endif
