#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"
#include "../tests/test_entity_all.cpp"

using namespace snek;
using namespace snek::internal;

int main(int argc, char **argv)
{
    int x;
    TEST_ENTITY_ALL();
    return 0;
}