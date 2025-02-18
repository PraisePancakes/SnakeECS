#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
class A
{
};
int main(int argc, char **argv)
{
    static_assert(snek::internal::has_cmp_mask_v<snek::Entity, u64>, "Entity must have cmp_mask");
    static_assert(snek::internal::has_tag_v<snek::Entity, std::string>, "Entity must have cmp_mask");
    return 0;
}