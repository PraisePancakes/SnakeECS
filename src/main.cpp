#include <iostream>
#include "../include/ecs/component.hpp"
#include "../include/ecs/entity.hpp"

using namespace snek;
using namespace core;

int main(int argc, char **argv)
{
    Entity e;

    A b = e.AddComponent<A>();
    std::cout << std::boolalpha << e.HasComponent<A>() << std::endl;
    e.RemoveComponent<A>();
    std::cout << std::boolalpha << e.HasComponent<A>() << std::endl;
    return 0;
}