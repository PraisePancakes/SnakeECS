#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"

using namespace snek;
using namespace snek::internal;

int main(int argc, char **argv)
{
    World<Entity, 1000> w;
    Entity e = w.Spawn("test");
    auto tag = w.GetEntityByID(e.GetID())->GetTag();
    std::cout << tag;

        return 0;
}