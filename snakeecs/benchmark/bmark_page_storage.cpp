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
    snek::storage::page_storage<int, std::allocator<int>> ps;

    for (size_t i = 0; i < 10; i++)
    {
        vec.push_back(i);
    }

    for (size_t i = 0; i < 10; i++)
    {
        ps.insert(i, i);
    }

    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < 10; i++)
    {
        auto x = vec[i];
        std::cout << x << std::endl;
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "benchmark vector get : " << ms_double.count() << "ms\n";

    auto t3 = high_resolution_clock::now();
    for (size_t i = 0; i < 10; i++)
    {
        auto x = ps.get(i);
    }
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> ms_double2 = t4 - t3;

    std::cout << "benchmark page storage get: " << ms_double2.count() << "ms\n";
}

void BMARK_PAGE_STORAGE_POP()
{
    std::vector<int> vec;
    snek::storage::page_storage<int, std::allocator<int>> ps;

    for (size_t i = 0; i < 10; i++)
    {
        vec.push_back(i);
    }

    for (size_t i = 0; i < 10; i++)
    {
        ps.insert(i, i);
    }

    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < 10; i++)
    {
        vec.pop_back();
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "benchmark vector pop : " << ms_double.count() << "ms\n";

    auto t3 = high_resolution_clock::now();
    for (size_t i = 0; i < 10; i++)
    {
        ps.pop();
    }
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> ms_double2 = t4 - t3;

    std::cout << "benchmark page storage pop: " << ms_double2.count() << "ms\n";
}

void BMARK_PAGE_STORAGE_FLAT_ITERATOR()
{

    snek::storage::page_storage<int, std::allocator<int>> ps;
    std::cout << "FLAT ITERATOR (REGULAR)" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        ps.insert(i, i);
    }

    for (auto it = ps.begin(); it != ps.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;
}

void BMARK_PAGE_STORAGE_FLAT_REVERSE_ITERATOR()
{

    snek::storage::page_storage<int, std::allocator<int>> ps;
    std::cout << "FLAT ITERATOR (REVERSE)" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        ps.insert(i, i);
    }

    for (auto it = ps.rbegin(); it != ps.rend(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;
}

void BMARK_PAGE_STORAGE_FLAT_CONST_ITERATOR()
{

    snek::storage::page_storage<int, std::allocator<int>> ps;
    std::cout << "FLAT ITERATOR (CONST)" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        ps.insert(i, i);
    }

    for (auto it = ps.cbegin(); it != ps.cend(); ++it)
    {
        //*it = 4;      //invalid
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;
}

void BMARK_PAGE_STORAGE_FLAT_CONST_REVERSE_ITERATOR()
{

    snek::storage::page_storage<int, std::allocator<int>> ps;
    std::cout << "FLAT ITERATOR (CONST REVERSE)" << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        ps.insert(i, i);
    }

    for (auto it = ps.crbegin(); it != ps.crend(); ++it)
    {
        //*it = 4; // invalid
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;
}