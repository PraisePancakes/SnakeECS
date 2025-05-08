#pragma once
#include "../debug/debug.hpp"

#ifdef _SNEK_WORLD_TEST_
#include <iostream>
#include <sstream>
#include "../ecs/world.hpp"
#include "configuration_policy.hpp"

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
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "TEST COMPONENT WITH MULTIPLE ARGS");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.bind<component_c>(e, 5, 'c', "COOL");
        int a = w.get_ref<component_c>(e).a;
        char b = w.get_ref<component_c>(e).b;
        std::string c = w.get_ref<component_c>(e).c;
        LOG("ENTITY " + std::to_string(e) + " C(a) " + std::to_string(a) + " C(b) " + b + " C(c) " + c);
    }

    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", "WORLD INITIALIZE A & B");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.initialize<component_a, component_b>(e, 5, 2);
        int a = w.get_ref<component_a>(e).x;
        int b = w.get_ref<component_b>(e).x;
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
        w.initialize<component_a, component_b>(e, 9, 1);
        bool b = w.has_any<component_a, component_c>(e);
        LOG("ENTITY " + std::to_string(e) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-79) WORLD CONTAINS ENTITIES  TRUE");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 79; i++)
    {
        bool b = w.contains(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (70-100) WORLD CONTAINS ENTITIES  FALSE");
    LOG(".\n.\n.");
    for (size_t i = 80; i < 100; i++)
    {
        bool b = w.contains(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }
    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-10) WORLD GET COMPONENT<A>:X=4");
    LOG(".\n.\n.");
    for (size_t i = 0; i < 10; i++)
    {
        int x = w.get_ref<component_a>(i).x;
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
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (0-79) WORLD CONTAINS ENTITY TRUE");
    LOG(".\n.\n.");

    for (size_t i = 0; i < 80; i++)
    {
        bool b = w.contains(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }

    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " (80-100) WORLD CONTAINS ENTITY FALSE");
    LOG(".\n.\n.");
    for (size_t i = 80; i < 100; i++)
    {
        bool b = w.contains(i);
        LOG("ENTITY " + std::to_string(i) + " " + bool_as_text(b));
    }

    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " WORLD COMPONENT VIEW A & B");
    LOG(".\n.\n.");

    auto v = w.view<component_a, component_b>();
    v.for_each([](component_a &a, component_b &b)
               {
                int ax =  a.x;
                int bx = b.x;
                LOG("A : " + std::to_string(ax) + " B : " + std::to_string(bx)); });

    std::cout << std::endl;

    LOG("\n.\n.\n.\n.\n.\n.");
    LOG_DESCRIPTION("SNEK_TEST", "TEST_WORLD", " WORLD ENTITY VERSIONS");
    LOG(".\n.\n.");
    LOG("EXPECTED BEFORE : 0");
    for (size_t i = 0; i < 10; i++)
    {
        LOG("ENTITY ID : " + std::to_string(w.to_id(i)) + " VERSION : " + std::to_string(w.to_version(i)));
    }

    LOG("EXPECTED AFTER : 1");
    for (size_t i = 0; i < 10; i++)
    {
        w.kill(i);
        LOG("ENTITY ID : " + std::to_string(w.to_id(i)) + " VERSION : " + std::to_string(w.to_version(i)));
    }
    std::cout << std::endl;
}

#endif