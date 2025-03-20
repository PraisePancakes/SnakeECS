#pragma once
#include "../core/verbose_allocator.hpp"
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <array>
#include "../utils/utils.hpp"
#include "../utils/identifier.hpp"
#include "../core/storage.hpp"
#include "entity.hpp"
#include "../debug/debug.hpp"
#include "world_policy.hpp"

namespace snek
{

    template <typename Policy>
    class world
    {
       

    public:
        using world_policy = Policy;

    private:
        using entity_type = Policy::entity_index_t;
        using component_list = Policy::component_list;
        using identifier_type = snek::uuid::identifier<Policy>;

        snek::storage::page_storage<entity_type> entity_store;

    public:
        world() : entity_store() {};
        [[nodiscard]] entity_type spawn()
        {
            entity_type id = identifier_type::generate_entity_id();
            entity_store.insert(id, id);
            return id;
        };
        [[nodiscard]] bool contains(entity_type id)
        {
            return entity_store.get(id) != snek::utils::tombstone_t<entity_type>::null_v;
        }
        ~world() {};
    };
}
