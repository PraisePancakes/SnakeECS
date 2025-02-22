#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"

using namespace snek;
using namespace snek::internal;

struct A
{
};
int main(int argc, char **argv)
{
    World w;
    auto e = w.Spawn();
    Entity b;

    std::cout << std::boolalpha << w.HasEntity(b);

    return 0;
}