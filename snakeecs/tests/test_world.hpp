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

    std::cout << "[WORLD INITIALIZE A : B]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a, component_b>(e, 7, 8);
        std::cout << "a : " << w.get<component_a>(e).x << " b : " << w.get<component_b>(e).x << std::endl;
    }
    std::cout << "[WORLD HAS ANY <A, B> {true}]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a>(e, 7);
        std::cout << std::boolalpha << " " << w.has_any<component_a, component_b>(e) << std::endl;
    }

    std::cout << "[WORLD HAS ANY <A, C> {true : has a}]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a>(e, 7);
        std::cout << std::boolalpha << " " << w.has_any<component_a, component_c>(e) << std::endl;
    }

    std::cout << "[WORLD HAS ANY <C, D> {false : has a, b}]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a>(e, 7);
        std::cout << std::boolalpha << " " << w.has_any<component_c, component_d>(e) << std::endl;
    }

    std::cout << "[WORLD HAS ALL <A, B> {true : has a, b}]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a, component_b>(e, 7, 8);
        std::cout << std::boolalpha << " " << w.has_all<component_a, component_b>(e) << std::endl;
    }

    std::cout << "[WORLD HAS ALL <A, C> {false : has a, b}]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a, component_b>(e, 7, 8);
        std::cout << std::boolalpha << " " << w.has_all<component_a, component_c>(e) << std::endl;
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

    std::cout << "[WORLD SIZE BEFORE KILL = 70]" << std::endl;
    std::cout << w.size() << std::endl;

    std::cout << "[WORLD KILL (false)]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.kill(e);
        std::cout << std::boolalpha << w.contains(e) << std::endl;
    }

    std::cout << "[WORLD KILL check components (false)]" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.bind<component_a>(e);
        w.kill(e);
        std::cout << std::boolalpha << w.has<component_a>(e) << std::endl;
    }

    std::cout << "[WORLD SIZE AFTER KILL = 70]" << std::endl;
    std::cout << w.size() << std::endl;

    std::cout << std::endl;
}

#endif