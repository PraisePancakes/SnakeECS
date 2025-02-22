#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"

using namespace snek;
using namespace snek::internal;

struct A
{
};
int main(int argc, char **argv)
{

    bool v = has_entt_traits<Entity, std::string, u64>::value;
    std::cout << std::boolalpha << v << std::endl;
    return 0;
}