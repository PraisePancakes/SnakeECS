#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"

using namespace snek;
using namespace snek::internal;

class A : public Component
{
};

class B : public Component
{
};

class C : public Component
{
};

int main(int argc, char **argv)
{
    World w;

    Entity e = w.Spawn();

    e.AddComponent<A>();
    e.AddComponent<B>();

    std::cout
        << std::boolalpha << e.HasComponent<A, B>() << std::endl;

    return 0;
}