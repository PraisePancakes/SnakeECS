#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"

using namespace snek;
using namespace snek::internal;

struct A : public Component
{
    int x = 2;
    A() {};
    A(int x) : x(x) {};
};

int main(int argc, char **argv)
{


    return 0;
}