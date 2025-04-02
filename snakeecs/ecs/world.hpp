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
        snek::storage::page_storage<entity_type, allocator_type> entities;
        std::vector<snek::storage::polymorphic_sparse_set *, allocator_type> _component_pools;

        allocator_type alloc;

    public:
        world() : alloc(), entities(), entity_store()
        {
            _component_pools.resize(component_list::size, nullptr);
        };
        world(allocator_type &a) : alloc(a), entities(), entity_store() {};
        [[nodiscard]] entity_type spawn()
        {
            entity_type id = world_policy::generate_entity_id();
            if (id >= snek::traits::tombstone_t<entity_type>::null_v - 1) [[unlikely]]
            {
                if (entity_store.size() > 0)
                {
                    // retrieve the back entity

                    // pop strictly the back element not the page
                };
            }
            entities.insert(id);
            return id;
        };
        [[nodiscard]] bool contains(entity_type id)
        {
            return entities.get(id) != snek::traits::tombstone_t<entity_type>::null_v;
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
