#include <iostream>
#include "snakeecs/benchmark/bmark_page_storage.cpp"
#include "snakeecs/utils/traits/snek_traits.hpp"
#include "snakeecs/ecs/world_policy.hpp"
#include "snakeecs/ecs/world.hpp"
#include "snakeecs/config/configuration_policy.hpp"
#include <map>

int main(int argc, char **argv)
{

    snek::world_policy<int, snek::test_config::component_types, std::allocator<int>> policy;

    return 0;
}