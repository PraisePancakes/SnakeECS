#include <iostream>
#include "snakeecs/ecs/world.hpp"
#include <tuple>
#include "snakeecs/config/configuration_policy.hpp"
#include <algorithm>
#include <chrono>

using namespace snek;
using namespace config;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main(int argc, char **argv)
{

    std::vector<int> vec;
    snek::storage::page_storage<int> vec1;

    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < 9000; i++)
    {
        vec.push_back(i);
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "benchmark vector : " << ms_double.count() << "ms\n";

    auto t3 = high_resolution_clock::now();
    for (size_t i = 0; i < 9000; i++)
    {
        vec1.insert(i, i);
    }
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> ms_double2 = t4 - t3;

    std::cout << "benchmark page storage : " << ms_double2.count() << "ms\n";

    return 0;
}