#include <iostream>
#include "snakeecs/benchmark/bmark_page_storage.cpp"
#include "snakeecs/utils/traits/snek_traits.hpp"
#include "snakeecs/ecs/world_policy.hpp"
#include "snakeecs/ecs/world.hpp"
#include "snakeecs/config/configuration_policy.hpp"
#include <map>
#include "snakeecs/core/storage.hpp"

struct A
{
    int x;
    A(int x) : x(x) {};
    ~A() {};
};

int main(int argc, char **argv)
{

    snek::world<snek::test_config::configuration_policy> world;
    snek::storage::sparse_set<int> ss;
    ss.insert(1, 3);
    std::cout << std::boolalpha << ss.contains(1) << std::endl;

    return 0;
}