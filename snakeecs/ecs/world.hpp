#pragma once
#include "../core/verbose_allocator.hpp"
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <array>
#include "../utils/utils.hpp"
#include "../utils/identifier.hpp"
#include "entity.hpp"
#include "../debug/debug.hpp"

namespace snek
{

#define PAGE_SIZE 4

    template <typename EntityT, typename ComponentList>
    struct world_policy;

    template <typename EntityT, typename ComponentList>
    struct world_policy
    {
        using entity_index_t = EntityT;
        using component_list = ComponentList;

        template <typename C>
        static constexpr size_t get_component_type_id()
        {
            return snek::utils::index_of<C, component_list>();
        };
    };

    template <typename T>
    class page_storage
    {

        std::vector<std::array<T, PAGE_SIZE>> pages;
        static constexpr auto tombstone_v = snek::tombstone_t<T>::null_v;
        void resize(size_t new_size)
        {
            std::array<T, PAGE_SIZE> arr;
            arr.fill(tombstone_v);
            pages.resize(new_size, arr);
        }

    public:
        explicit page_storage(size_t size)
        {
            resize(size);
#ifdef _SNEK_DEBUG_
            LOG("basic storage T : " + snek::utils::type_descriptor<T>().stringify());
#endif
        };

        explicit page_storage()
        {
            resize(10);
#ifdef _SNEK_DEBUG_
            LOG("basic storage T : " + snek::utils::type_descriptor<T>().stringify());
#endif
        };

        void insert(size_t id, T what)
        {

            size_t page = id / PAGE_SIZE;
            size_t index = id % PAGE_SIZE;
            if (page >= pages.size())
            {
                resize(page * 2);
#ifdef _SNEK_DEBUG_
                LOG("basic storage T : " + snek::utils::type_descriptor<T>().stringify());
#endif
            }
            pages[page][index] = what;
        }

        [[nodiscard]] T get(size_t id)
        {
            size_t page = id / PAGE_SIZE;
            size_t index = id % PAGE_SIZE;

            if (page < pages.size() && pages[page][index] != tombstone_v)
            {
                return pages[page][index];
            }

            return tombstone_v;
        }

        ~page_storage() {};
    };

    template <typename Policy>
    class world
    {
        using world_policy = Policy;
        using entity_type = Policy::entity_index_t;
        using component_list = Policy::component_list;
        using identifier_type = snek::uuid::identifier<Policy>;
        static_assert(snek::entity::is_entity_type<entity_type>::value);

        page_storage<entity_type> entity_store;

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
            return entity_store.get(id) != snek::tombstone_t<entity_type>::null_v;
        }
        ~world() {};
    };
}
