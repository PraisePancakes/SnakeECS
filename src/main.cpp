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
    auto e = w.spawn("entity");
    std::cout << e.GetID();
    Entity b;
    std::cout << b.GetID();

    std::cout << std::boolalpha << w.has_entity(e);

    return 0;
}