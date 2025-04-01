#include <iostream>
#include "snakeecs/benchmark/bmark_page_storage.cpp"
#include "snakeecs/utils/traits/snek_traits.hpp"
#include "snakeecs/ecs/world_policy.hpp"
#include "snakeecs/ecs/world.hpp"
#include "snakeecs/config/configuration_policy.hpp"
#include <map>
#include "snakeecs/benchmark/bmark_sparse_set.hpp"
#include "snakeecs/core/storage/sparse_set.hpp"

struct A
{
    int x;
    A(int x) : x(x) {};
    ~A() {};
};

int main(int argc, char **argv)
{
    BMARK_SPARSE_SET();
    BMARK_SPARSE_SET_ITERATOR();

    return 0;
}