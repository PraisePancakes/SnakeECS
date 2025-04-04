#pragma once
#include "../debug/debug.hpp"

#ifdef _SNEK_WORLD_TEST_
#include <iostream>
#include "../ecs/world.hpp"
#include "../config/configuration_policy.hpp"

using namespace snek;
using namespace test_config;

void TEST_WORLD_MULTIPLE_ENTITIES()
{
    std::cout << "WORLD TEST MULTIPLE ENTITIES" << std::endl;
    snek::world<test_config::configuration_policy> w;
    std::cout << "[WORLD SPAWNED ENTITIES]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        std::cout << e << std::endl;
        w.bind<component_a>(e);
    }
    std::cout << "[WORLD CONTAINS ENTITIES]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        std::cout << i << std::boolalpha << " " << w.contains(i) << std::endl;
    }
    std::cout << "[WORLD GET COMPONENT A (x = 4)]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto c = w.get<component_a>(i);
        std::cout << c.x << std::endl;
    }
    std::cout << "[WORLD HAS COMPONENT A (true)]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {

        std::cout << std::boolalpha << w.has<component_a>(i) << std::endl;
    }

    std::cout << "[WORLD HAS COMPONENT B (false)]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {

        std::cout << std::boolalpha << w.has<component_b>(i) << std::endl;
    }

    std::cout << std::endl;
}

#endif