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

    // std::uint64_t e = 0;

    // for (size_t i = 1; i <= 11; i++)
    // {
    //     e = test_config::configuration_policy::generate_entity_id();
    // }

    // std::cout << "ID : " << e << std::endl;
    // std::cout << "Entity : " << test_config::configuration_policy::to_entity(e) << std::endl;
    // std::cout << "Version : " << test_config::configuration_policy::to_version(e) << std::endl;
    // test_config::configuration_policy::increment_version(e);
    // std::cout << "New Version : " << test_config::configuration_policy::to_version(e) << std::endl;
    // test_config::configuration_policy::increment_version(e);
    // std::cout << "New Version : " << test_config::configuration_policy::to_version(e) << std::endl;
    // test_config::configuration_policy::increment_version(e);
    // std::cout << "New Version : " << test_config::configuration_policy::to_version(e) << std::endl;
    // test_config::configuration_policy::increment_version(e);
    // std::cout << "New Version : " << test_config::configuration_policy::to_version(e) << std::endl;
    // std::cout << e << std::endl;
    return 0;
}