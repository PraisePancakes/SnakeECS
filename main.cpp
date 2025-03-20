#include <iostream>
#include "snakeecs/ecs/world.hpp"
#include <tuple>

int main(int argc, char **argv)
{
    using ComponentTypes = snek::component_list<int, char, size_t>;
    using policy = snek::world_policy<std::uint64_t, ComponentTypes>;

    snek::world<policy> world;
    auto id = world.spawn();

    return 0;
}