#include <iostream>
#include "../core/storage/page_storage.hpp"
#include <algorithm>
#include <chrono>

using namespace snek;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

void BMARK_PAGE_STORAGE_INSERT()
{
    std::vector<int> vec;
    snek::storage::page_storage<int, std::allocator<int>> vec1;

    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < 9000; i++)
    {
        vec.push_back(i);
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "benchmark vector push : " << ms_double.count() << "ms\n";

    auto t3 = high_resolution_clock::now();
    for (size_t i = 0; i < 9000; i++)
    {
        vec1.insert(i, i);
    }
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> ms_double2 = t4 - t3;

    std::cout << "benchmark page storage insert: " << ms_double2.count() << "ms\n";
};

void BMARK_PAGE_STORAGE_GET()
{
    std::vector<int> vec;
    snek::storage::page_storage<int, std::allocator<int>> vec1;

    for (size_t i = 0; i < 9000; i++)
    {
        vec.push_back(i);
    }

    for (size_t i = 0; i < 9000; i++)
    {
        vec1.insert(i, i);
    }

    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < 9000; i++)
    {
        vec[i];
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "benchmark vector get : " << ms_double.count() << "ms\n";

    auto t3 = high_resolution_clock::now();
    for (size_t i = 0; i < 9000; i++)
    {
        vec1.get(i);
    }
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> ms_double2 = t4 - t3;

    std::cout << "benchmark page storage get: " << ms_double2.count() << "ms\n";
}