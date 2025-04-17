#include <iostream>
#include "snakeecs/tests/test_world_policy.hpp"
#include "snakeecs/tests/test_component.hpp"
#include "snakeecs/tests/test_world.hpp"
#include "snakeecs/tests/configuration_policy.hpp"
#include "snakeecs/debug/debug.hpp"
#include <map>
#include "snakeecs/benchmark/bmark_sparse_set.hpp"

int main(int argc, char **argv)
{

#if _SNEK_BENCHMARK_
    BMARK_SPARSE_SET();
    BMARK_SPARSE_SET_ITERATOR();
    std::cout << std::endl;

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

    snek::world<snek::test_config::configuration_policy> w;

    // for (size_t i = 0; i < 10; i++)
    // {
    //     auto e = w.spawn();
    //     w.bind<component_a>(e, 1);
    //     w.bind<component_b>(e, 0);
    // }

    // auto v = w.view<component_a, component_b>();

    // for (auto it = v.cbegin(); it != v.cend(); ++it)
    // {
    //     *it = 6;
    // }

    return 0;
}