
#include "../include/ecs/entity_traits.hpp"
#include <iostream>

static void TEST_ENTITY_TRAITS()
{
    snek::entity_traits<std::uint64_t>::value_type e0;
    snek::entity_traits<std::uint32_t>::value_type e1;
};