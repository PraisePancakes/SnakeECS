#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "../utils/type_list.hpp"
#include "../core/storage/page_storage.hpp"
#include "../core/storage/sparse_set.hpp"
#include "../debug/debug.hpp"
#include "world_policy.hpp"
#include "traits.hpp"

namespace snek
{

    template <typename Policy>
    class world
    {

    public:
        using world_policy = Policy;

    private:
        using entity_type = Policy::entity_index;
        using component_list = Policy::component_list;
        using allocator_type = Policy::allocator_type;

        snek::storage::page_storage<entity_type, allocator_type> entity_store;
        std::vector<snek::storage::polymorphic_sparse_set *> _component_pools;

        allocator_type alloc;

    public:
        world() : entity_store()
        {
            _component_pools.resize(component_list::size);
        };
        world(allocator_type &a) : alloc(a) {};
        [[nodiscard]] entity_type spawn()
        {
            entity_type id = world_policy::generate_entity_id();
            entity_store.insert(id, id);
            return id;
        };
        [[nodiscard]] bool contains(entity_type id)
        {
            return entity_store.get(id) != snek::traits::tombstone_t<entity_type>::null_v;
        }

        template <typename C, typename... Args>
        C &bind(entity_type e, Args &&...args)
        {
            static_assert(world_policy::template is_valid_component<C>(), "C must be a registered component. ");
        }

        [[nodiscard]] allocator_type &get_allocator() const noexcept
        {
            return alloc;
        }
        ~world() {};
    };
}
