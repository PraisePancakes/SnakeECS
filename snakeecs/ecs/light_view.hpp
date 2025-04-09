#pragma once
#include "../core/storage/page_storage.hpp"
#include "../core/storage/sparse_set.hpp"
namespace snek
{
    namespace ecs
    {
        template <typename World, typename... Ts>
        class light_view
        {
            using world_policy = World::world_policy;
            using entity_type = world_policy::entity_index;
            using component_list = world_policy::component_list;
            using allocator_type = world_policy::allocator_type;
            std::vector<snek::storage::polymorphic_sparse_set *> _component_pools;

        public:
            light_view(const std::vector<snek::storage::polymorphic_sparse_set *> &cp) : _component_pools(cp) {};
            ~light_view() {};
        };
    }
}