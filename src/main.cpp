#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"

using namespace snek;
using namespace snek::internal;

int main(int argc, char **argv)
{
    World w;
    auto e = w.Spawn();
    auto b = w.Spawn();

    std::cout << w.GetEntitiesByTag("").size();
    return 0;
}