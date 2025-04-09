#pragma once
#include "../debug/debug.hpp"

#ifdef _SNEK_WORLD_TEST_
#include <iostream>
#include "../ecs/world.hpp"
#include "../config/configuration_policy.hpp"

using namespace snek;
using namespace test_config;

std::string bool_as_text(bool b)
{
    std::stringstream converter;
    converter << std::boolalpha << b; // flag boolalpha calls converter.setf(std::ios_base::boolalpha)
    return converter.str();
}

void TEST_WORLD_MULTIPLE_ENTITIES()
{
    std::cout << "WORLD TEST MULTIPLE ENTITIES" << std::endl;
    snek::world<test_config::configuration_policy> w;
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "SPAWNING ENTITIES");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        LOG("ENTITY " + std::to_string(e));
        w.bind<component_a>(e);
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "WORLD INITIALIZE A & B");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a, component_b>(e, 7, 8);
        int a = w.get<component_a>(e).x;
        int b = w.get<component_b>(e).x;
        LOG("ENTITY " + std::to_string(e) + " A " + std::to_string(a) + " B " + std::to_string(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "WORLD HAS <A>:TRUE");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a>(e, 7);
        bool b = w.has<component_a>(e);
        LOG("ENTITY " + std::to_string(e) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "WORLD HAS ANY <A, C>:TRUE HAS A & B");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a>(e, 7);
        bool b = w.has_any<component_a, component_b>(e);
        LOG("ENTITY " + std::to_string(e) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "WORLD HAS ANY <C, D>:FALSE HAS A & B");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a>(e, 7);
        bool b = w.has_any<component_c, component_d>(e);
        LOG("ENTITY " + std::to_string(e) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "WORLD HAS ALL <A, B>:TRUE HAS A & B");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a, component_b>(e, 7, 8);
        bool b = w.has_any<component_a, component_b>(e);
        LOG("ENTITY " + std::to_string(e) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "WORLD HAS ALL <A, C>:FALSE HAS A & B");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a, component_b>(e, 7, 8);
        bool b = w.has_any<component_a, component_c>(e);
        LOG("ENTITY " + std::to_string(e) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-69) WORLD CONTAINS ENTITIES  TRUE");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 70; i++)
    {
        bool b = w.contains(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (70-100) WORLD CONTAINS ENTITIES  FALSE");
    LOG(".\n.\n.");
    for (size_t i = 70; i < 100; i++)
    {
        bool b = w.contains(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-10) WORLD GET COMPONENT<A>:X=4");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        int x = w.get<component_a>(i).x;
        LOG("ENTITY " + std::to_string(i) + " " + std::to_string(x));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-10) WORLD HAS COMPONENT<A>:TRUE");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        bool b = w.has<component_a>(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-10) WORLD HAS COMPONENT<B>:FALSE");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        bool b = w.has<component_b>(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-10) WORLD  SPAWN KILL CONTAINS ENTITY FALSE");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.kill(e);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(w.contains(e)));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-10) WORLD SPAWN KILL HAS COMPONENT<A>:FALSE");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.bind<component_a>(e);
        w.kill(e);
        bool b = w.has<component_a>(e);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-69) WORLD CONTAINS ENTITY TRUE");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 70; i++)
    {
        bool b = w.contains(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }

    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (70-100) WORLD CONTAINS ENTITY FALSE");
    LOG(".\n.\n.");
    for (size_t i = 70; i < 100; i++)
    {
        bool b = w.contains(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }

    std::cout << std::endl;
}

#endif