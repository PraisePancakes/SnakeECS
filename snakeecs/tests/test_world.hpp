#pragma once
#include "../debug/debug.hpp"

#ifdef _SNEK_WORLD_TEST_
#include <iostream>
#include "../ecs/world.hpp"
#include "../config/configuration_policy.hpp"

using namespace snek;
using namespace test_config;

void TEST_WORLD_COMPONENT()
{

    std::cout << "WORLD TEST COMPONENT" << std::endl;
    snek::world<test_config::configuration_policy> world;
    auto e = world.spawn();
    world.bind<snek::test_config::component_a>(e);
    auto v = world.get<snek::test_config::component_a>(e);
    std::cout << "COMPONENT VALUE" << std::endl;
    std::cout << v.x << std::endl;
    std::cout << std::endl;
};

void TEST_WORLD_CONTAINS_ENTITY()
{
    std::cout << "WORLD TEST CONTAINS ENTITY" << std::endl;
    snek::world<test_config::configuration_policy> world;

    std::cout << "TRUE CASE : " << std::endl;
    auto e = world.spawn();
    std::cout << std::boolalpha << world.contains(e) << std::endl;
    std::cout << std::endl;

    std::cout << "FALSE CASE : " << std::endl;
    std::cout << std::boolalpha << world.contains(1) << std::endl;
    std::cout << std::endl;
}

void TEST_WORLD_CONTAINS_COMPONENT()
{
    std::cout << "WORLD TEST CONTAINS COMPONENT" << std::endl;

    std::cout << "TRUE CASE : " << std::endl;
    snek::world<test_config::configuration_policy> world;
    auto e = world.spawn();
    world.bind<snek::test_config::component_a>(e);
    std::cout << std::boolalpha << world.has<snek::test_config::component_a>(e) << std::endl;

    std::cout << "FALSE CASE : " << std::endl;
    world.bind<snek::test_config::component_b>(e);
    std::cout << std::boolalpha << world.has<snek::test_config::component_a>(e) << std::endl;
    std::cout << std::endl;
}

#endif