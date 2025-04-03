#pragma once
#include "../debug/debug.hpp"

#ifdef _SNEK_WORLD_TEST_
#include <iostream>
#include "../ecs/world.hpp"
#include "../config/configuration_policy.hpp"

using namespace snek;
using namespace test_config;

void TEST_WORLD()
{
    std::cout << "WORLD TEST" << std::endl;
    snek::world<test_config::configuration_policy> world;
    auto e = world.spawn();
    world.bind<snek::test_config::component_a>(e);
    auto v = world.get<snek::test_config::component_a>(e);
    std::cout << std::boolalpha << world.has<snek::test_config::component_a>(e) << std::endl;
    std::cout << v.x << std::endl;
    std::cout << std::endl;
};
#endif