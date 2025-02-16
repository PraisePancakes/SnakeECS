#include <iostream>
#include "../include/ecs/component.hpp"
#include "../include/ecs/entity.hpp"

using namespace snek;


int main(int argc, char **argv)
{
    Entity e;
    e.AddComponent<A>();

    return 0;
}