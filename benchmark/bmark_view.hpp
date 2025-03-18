#pragma once
#include "../include/ecs/world.hpp"
#include <chrono>

class bmark_A
{
    int a = 2;
};
class bmark_B
{
    int x = 3;
};

void bmark_view()
{
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    snek::world<5000> w;

    for (size_t i = 0; i < w.size(); i++)
    {
        auto e = w.spawn();
        w.bind<bmark_A>(e);
        w.bind<bmark_B>(e);
    }
    auto view = w.view<bmark_A, bmark_B>();

    auto t1 = high_resolution_clock::now();
    view.for_each([](const bmark_A &a, const bmark_B &b) {});
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "view benchmark before optimization : " << ms_double.count() << "ms\n";
};
