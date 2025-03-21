#pragma once
#include "../core/verbose_allocator.hpp"
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <array>
#include "../utils/type_list.hpp"
#include "../core/storage.hpp"
#include "entity.hpp"
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

        snek::storage::page_storage<entity_type> entity_store;

    public:
        world() : entity_store() {};
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
        ~world() {};
    };
}
