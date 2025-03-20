#pragma once
#include "testing_config.hpp"

#ifdef _SNEK_WORLD_POLICY_STATIC_TEST_
#include <iostream>
#include "../ecs/world_policy.hpp"
#include "../config/configuration_policy.hpp"

using namespace config;
using namespace test_config;

class NonListComponent
{
};

struct Test
{
    static_assert(!configuration_policy::is_valid_component<NonListComponent>());
    static_assert(configuration_policy::is_valid_component<ComponentB>());
    static_assert(configuration_policy::is_valid_component<ComponentA>());
};

#endif