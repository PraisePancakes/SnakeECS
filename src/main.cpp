#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
class A
{
};
int main(int argc, char **argv)
{

    // tag must be std::string so resulting type is void;
    static_assert(std::is_void_v<snek::internal::has_tag<snek::Entity, u64>::tag_type>, "has tag");
    // tag must be std::string so resulting type is std::string;
    static_assert(!std::is_void_v<snek::internal::has_tag<snek::Entity, std::string>::tag_type>, "has tag");
    static_assert(std::is_same_v<snek::internal::has_tag<snek::Entity, std::string>::tag_type, std::string>, "is string type");
    static_assert(!snek::internal::has_tag<A, std::string>::value, "doesnt have tag");
    static_assert(snek::internal::has_entity_traits<snek::Entity>::value, "has traits");
    static_assert(!snek::internal::has_entity_traits<A>::value, "doesnt have traits");

    return 0;
}