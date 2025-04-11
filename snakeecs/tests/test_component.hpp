#pragma once
#include "../debug/debug.hpp"

#ifdef _SNEK_STATIC_TEST_
#include <iostream>
#include "../ecs/world.hpp"
#include "configuration_policy.hpp"

using namespace snek;
using namespace test_config;

void TEST_STATIC_WORLD_POLICY()
{
    // A : 0 , B : 1
    // assume component_a and component_b are registered in world policy
    static_assert(configuration_policy::get_component_type_id<component_a>() == 0);
    static_assert(configuration_policy::get_component_type_id<component_b>() != 0);

    static_assert(configuration_policy::get_component_type_id<component_b>() == 1);
    static_assert(configuration_policy::get_component_type_id<component_a>() != 1);
};
#endif
