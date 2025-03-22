#pragma once
#include "testing_config.hpp"

#ifdef _SNEK_WORLD_POLICY_STATIC_TEST_
#include <iostream>
#include <vector>
#include "../ecs/world_policy.hpp"
#include "../config/configuration_policy.hpp"
#include "../utils/traits/snek_traits.hpp"

using namespace snek;
using namespace test_config;

namespace TEST1
{
    class not_registered_component
    {
    };
    // NonListComponent is not a part of the configuration policy therefore it is not a valid registered component :
    // refer to snakeecs/config/configuration_policy.hpp specifically config::test_config::component_types.

    static_assert(!configuration_policy::is_valid_component<not_registered_component>());
    static_assert(configuration_policy::is_valid_component<component_a>());
    static_assert(configuration_policy::is_valid_component<component_b>());
};

namespace TEST2
{
    static_assert(snek::traits::has_value_type_v<std::vector<int>>, "has value_type typedef");
    static_assert(!snek::traits::has_value_type_v<int>, "doesnt have value_type typedef");
    static_assert(snek::traits::has_allocate<std::allocator<int>>::value, "has allocate method that accepts size_t");
    static_assert(!snek::traits::has_allocate<int>::value, "doesnt have allocate method that accepts size_t");
};

#endif