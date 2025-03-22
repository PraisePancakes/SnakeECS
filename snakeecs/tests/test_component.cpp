
#include "testing_config.hpp"

#ifdef _SNEK_COMPONENT_TYPE_STATIC_TEST_
#include <iostream>
#include "../ecs/world.hpp"
#include "../config/configuration_policy.hpp"

using namespace snek;
using namespace test_config;

struct test
{
    // A : 0 , B : 1
    // assume component_a and component_b are registered in world policy
    static_assert(configuration_policy::get_component_type_id<component_a>() == 0);
    static_assert(!configuration_policy::get_component_type_id<component_b>() == 0);

    static_assert(configuration_policy::get_component_type_id<component_b>() == 1);
    static_assert(!configuration_policy::get_component_type_id<component_a>() == 1);
};
#endif
