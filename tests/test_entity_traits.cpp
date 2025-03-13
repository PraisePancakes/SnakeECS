
#include "../include/ecs/entity.hpp"
#include <iostream>

static void TEST_ENTITY_TRAITS()
{
    snek::entity_traits<std::uint64_t>::value_type e0;
    static_assert(std::is_same_v<decltype(e0), uint64_t>, "same!");
    static_assert(!std::is_same_v<decltype(e0), uint32_t>, "not same!");
    snek::entity_traits<std::uint32_t>::value_type e1;
    static_assert(!std::is_same_v<decltype(e1), uint64_t>, "same!");
    static_assert(std::is_same_v<decltype(e1), uint32_t>, "not same!");
};