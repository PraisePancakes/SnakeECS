#include "test_entity.cpp"
#include "test_entity_traits.cpp"
void TEST_ENTITY_ALL()
{
    TEST_ENTITY_TRAITS();
    TEST_VIEW();
    TEST_COMPONENT_LIST_INITIALIZER();
    TEST_COMPONENTS();
    TEST_ENTITY_ID();
    TEST_WORLD_FOR_EACH_STRICTLY();
}