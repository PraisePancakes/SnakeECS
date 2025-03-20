#include <iostream>
#include "../ecs/world.hpp"
#include "../config/configuration_policy.hpp"
#include "../config/testing_config.hpp"

#ifdef _SNEK_COMPONENT_TYPE_STATIC_TEST_
using namespace config;
using namespace test_config;
void test_static_components()
{

    // A : 0 , B : 1
    static_assert(configuration_policy::get_component_type_id<ComponentA>() == 0);
    static_assert(!configuration_policy::get_component_type_id<ComponentB>() == 0);

    static_assert(configuration_policy::get_component_type_id<ComponentB>() == 1);
    static_assert(!configuration_policy::get_component_type_id<ComponentA>() == 1);
};
#endif
