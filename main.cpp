#include <iostream>
#include "snakeecs/benchmark/bmark_page_storage.cpp"
#include "snakeecs/tests/test_world_policy.hpp"
#include "snakeecs/tests/test_component.hpp"
#include "snakeecs/tests/test_world.hpp"
#include "snakeecs/utils/traits/snek_traits.hpp"
#include "snakeecs/ecs/world_policy.hpp"
#include "snakeecs/ecs/world.hpp"
#include "snakeecs/config/configuration_policy.hpp"
#include "snakeecs/debug/debug.hpp"
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

#if _SNEK_BENCHMARK_
    BMARK_SPARSE_SET();
    BMARK_SPARSE_SET_ITERATOR();
    std::cout << std::endl;
    BMARK_PAGE_STORAGE_INSERT();
    BMARK_PAGE_STORAGE_GET();
    BMARK_PAGE_STORAGE_FLAT_ITERATOR();
    BMARK_PAGE_STORAGE_FLAT_REVERSE_ITERATOR();
    BMARK_PAGE_STORAGE_FLAT_CONST_REVERSE_ITERATOR();
    BMARK_PAGE_STORAGE_FLAT_CONST_ITERATOR();
    std::cout << std::endl;
#endif

#if _SNEK_DEBUG_
#if _SNEK_WORLD_TEST_
    TEST_WORLD_MULTIPLE_ENTITIES();
#endif

#if _SNEK_STATIC_TEST_
    TEST_STATIC_WORLD_POLICY();
    TEST_STATIC_CONFIG_POLICY();
    TEST_STATIC_UTIL_TYPE_TRAITS();
#endif
#endif //_SNEK_DEBUG_

    /*  SANDBOX */
    snek::world<configuration_policy> w;
    for (size_t i = 0; i < 10; i++)
    {
        auto e = w.spawn();
        w.bind<component_a>(e);
        w.bind<component_b>(e, 6);
    }
    auto view = w.view<component_a, component_b>();
    view.for_each([](component_a &a, component_b &b)
                  { std::cout << "HERE " << a.x << " B " << b.x << std::endl; });

    return 0;
}