#include <iostream>
#include "snakeecs/ecs/world.hpp"
#include <tuple>
#include "snakeecs/config/configuration_policy.hpp"

using namespace snek;
using namespace config;

int main(int argc, char **argv)
{

    snek::world<config::test_config::configuration_policy> world;
    auto id = world.spawn();
    std::cout << std::boolalpha << world.contains(id) << std::endl;
    std::cout << std::boolalpha << world.contains(2) << std::endl;

    return 0;
}