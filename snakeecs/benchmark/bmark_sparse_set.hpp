#include <iostream>
#include "../core/storage/sparse_set.hpp"
#include <algorithm>
#include <chrono>
#include <unordered_map>

using namespace snek;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

void BMARK_SPARSE_SET()
{
    snek::storage::sparse_set<int> ss;
    std::unordered_map<int, int> map;

    auto t1 = high_resolution_clock::now();

    for (size_t i = 0; i < 9000; i++)
    {
        ss.insert(i, i + 1);
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "benchmark sparse set insert : " << ms_double.count() << "ms\n";

    auto t3 = high_resolution_clock::now();

    for (size_t i = 0; i < 9000; i++)
    {
        map.insert(std::pair<int, int>(i, i + 1));
    }
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> ms_double1 = t4 - t3;
    std::cout << "benchmark map insert : " << ms_double1.count() << "ms\n";
};

void BMARK_SPARSE_SET_ITERATOR()
{
    snek::storage::sparse_set<std::vector<int>> ss;
    std::unordered_map<int, std::vector<int>> map;

    for (size_t i = 0; i < 9000; i++)
    {
        std::vector<int> vec(6, 100);
        ss.insert(i, vec);
    }

    for (size_t i = 0; i < 9000; i++)
    {
        std::vector<int> vec(6, 100);
        map.insert(std::pair<int, std::vector<int>>(i, vec));
    }

    auto t1 = high_resolution_clock::now();
    for (auto &el : ss)
    {
        for (auto &e : el)
        {
            std::cout << e << std::endl;
        }
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "benchmark sparse set iterator : " << ms_double.count() << "ms\n";

    auto t3 = high_resolution_clock::now();
    for (auto &el : map)
    {
        for (auto &e : el.second)
        {
            std::cout << e << std::endl;
        }
    }
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> ms_double1 = t4 - t3;
    std::cout << "benchmark map iterator : " << ms_double1.count() << "ms\n";
};
