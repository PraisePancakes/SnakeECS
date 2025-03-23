#include <iostream>
#include "snakeecs/benchmark/bmark_page_storage.cpp"
#include "snakeecs/utils/traits/snek_traits.hpp"
#include "snakeecs/ecs/world_policy.hpp"
#include "snakeecs/ecs/world.hpp"
#include "snakeecs/config/configuration_policy.hpp"
#include <map>
#include "snakeecs/core/storage.hpp"

int main(int argc, char **argv)
{

    snek::world<snek::test_config::configuration_policy> world;
    auto e = world.spawn();
    world.bind<snek::test_config::component_a>(e);

    snek::storage::sparse_set<int> sparse_set;


    return 0;
}