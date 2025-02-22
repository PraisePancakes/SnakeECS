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
    auto e = w.Spawn("entity");
    auto e_get = w.GetEntityByID(2);
    if (e_get)
    {
        std::cout << "id : " << e_get->GetID() << std::endl;
    };

    return 0;
}