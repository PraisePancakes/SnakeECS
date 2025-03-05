#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"
#include "../tests/entity.cpp"

using namespace snek;
using namespace snek::internal;

int main(int argc, char **argv)
{
    TEST_ENTITY_ID();
    TEST_COMPONENT_LIST_INITIALIZER();
    TEST_COMPONENT_GROUPS();

    return 0;
}