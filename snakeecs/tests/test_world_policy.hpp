#pragma once
#include "../debug/debug.hpp"

#ifdef _SNEK_STATIC_TEST_
#include <iostream>
#include <vector>
#include "../ecs/world_policy.hpp"
#include "configuration_policy.hpp"
#include "../utils/traits/snek_traits.hpp"

using namespace snek;
using namespace test_config;

class not_registered_component
{
};
// NonListComponent is not a part of the configuration policy therefore it is not a valid registered component :
// refer to snakeecs/config/configuration_policy.hpp specifically config::test_config::component_types.

void TEST_STATIC_CONFIG_POLICY()
{
    static_assert(!configuration_policy::is_valid_component<not_registered_component>());
    static_assert(configuration_policy::is_valid_component<component_a>());
    static_assert(configuration_policy::is_valid_component<component_b>());
}

#include <map>

void TEST_STATIC_UTIL_TYPE_TRAITS()
{
    static_assert(snek::traits::has_value_type_v<std::vector<int>>, "has value_type typedef");
    static_assert(!snek::traits::has_value_type_v<int>, "doesnt have value_type typedef");
    static_assert(snek::traits::has_allocate<std::allocator<int>>::value, "has allocate method that accepts size_t");
    static_assert(!snek::traits::has_allocate<int>::value, "doesnt have allocate method that accepts size_t");
    static_assert(snek::traits::tlist_has_size_constant<snek::test_config::component_types>::value, "has size constant");
    static_assert(!snek::traits::tlist_has_size_constant<int>::value, "doesnt have size constant");
    static_assert(!snek::traits::is_type_list<int>::value, "is not a type list");
    static_assert(!snek::traits::is_type_list<std::map<int, char>>::value, "is not a type list");
    static_assert(snek::traits::is_type_list<snek::test_config::component_types>::value, "is not a type list");
}

void TEST_STATIC_ENTITY_TRAITS()
{
    static_assert(snek::entity::is_entity_type<std::uint32_t>::value, "is entity type");
    static_assert(!snek::entity::is_entity_type<int>::value, "isn't entity type");
    static_assert(snek::entity::is_entity_type<std::uint64_t>::value, "is entity type");
}

#endif