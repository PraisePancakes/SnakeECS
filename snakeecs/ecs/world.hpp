#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "../utils/type_list.hpp"
#include "../core/storage/sparse_set.hpp"
#include "../debug/debug.hpp"
#include "world_policy.hpp"
#include "traits.hpp"
#include "light_view.hpp"
#include <queue>

namespace snek
{

    template <typename Policy>
    class world
    {

    public:
        using world_policy = Policy;
        using entity_type = Policy::entity_type;
        using component_list = Policy::component_list;
        using allocator_type = Policy::allocator_type;

    private:
        using this_type = world<Policy>;

        std::queue<entity_type> entity_store;
        std::vector<entity_type, allocator_type> entities;
        std::vector<snek::storage::polymorphic_sparse_set *> _component_pools;

        allocator_type alloc;

    public:
        world() : entity_store(), entities(), alloc()
        {
            _component_pools.resize(component_list::size, nullptr);
        };
        world(allocator_type &a) : alloc(a), entities(), entity_store() {};
        [[nodiscard]] entity_type spawn()
        {
            entity_type id = world_policy::generate_entity_id();
            bool overflowed = (id >= snek::traits::tombstone_t<entity_type>::null_v);

            if (overflowed) [[unlikely]]
            {
                // first check if the store is not empty
                if (!entity_store.empty())
                {
                    // retrieve the front entity
                    id = entity_store.front();
                    // pop strictly the front element
                    entity_store.pop();
                };
                // if its empty then no choice but to continue the ring of id's so back to 0 we go
            }

            entities.push_back(id);

            return id;
        };
        [[nodiscard]] bool contains(entity_type id)
        {
            return id < entities.size() &&
                   entities[id] != snek::traits::tombstone_t<entity_type>::null_v;
        }

        template <typename C>
        [[nodiscard]] bool has(entity_type e) const
        {
            SNEK_ASSERT(world_policy::template is_valid_component<C>(), "C must be a registered component. ");
            size_t c_id = world_policy::template get_component_type_id<C>();
            snek::storage::sparse_set<C> *ss = static_cast<snek::storage::sparse_set<C> *>(_component_pools[c_id]);
            if (ss)
            {
                return ss->contains(e);
            }
            return false;
        }

        template <typename T>
        [[nodiscard]] bool has_all(entity_type e) const noexcept
        {
            return has<T>(e);
        }

        template <typename T>
        [[nodiscard]] bool has_any(entity_type e) const noexcept
        {
            return has<T>(e);
        }

        template <typename T, typename U, typename... Args>
        [[nodiscard]] bool has_any(entity_type e) const noexcept
        {
            return has<T>(e) || has<U>(e) || (has<Args>(e) || ...);
        }

        template <typename T, typename U, typename... Args>
        [[nodiscard]] bool has_all(entity_type e) const noexcept
        {
            return (has<T>(e) && has<U>(e) && (has<Args>(e) && ...));
        }

        template <typename C, typename... Args>
        C &bind(entity_type e, Args &&...args)
        {

            SNEK_ASSERT(world_policy::template is_valid_component<C>(), "C must be a registered component. ");
            C *component = new C(std::forward<Args>(args)...);
            size_t c_id = world_policy::template get_component_type_id<C>();
            // TO DO ADD GUARD TO CHECK IF ENTITY HAS COMPONENT
            snek::storage::sparse_set<C> *ss = (snek::storage::sparse_set<C> *)_component_pools[c_id];
            if (ss)
            {
                if (has<C>(e))
                {
                    return *ss->get(e);
                }
            }
            else
            {
                ss = new snek::storage::sparse_set<C>();
            }
            ss->insert(e, *component);
            _component_pools[c_id] = ss;
            return *component;
        }

        template <typename... T, typename... Args>
        void initialize(entity_type e, Args &&...args)
        {
            (bind<T>(e, std::forward<Args>(args)), ...);
        }

        template <typename C>
        C &get(entity_type e)
        {
            SNEK_ASSERT(world_policy::template is_valid_component<C>(), "C must be a registered component. ");
            size_t c_id = world_policy::template get_component_type_id<C>();
            auto ss = static_cast<snek::storage::sparse_set<C> *>(_component_pools[c_id]);
            return ss->get_ref(e);
        }

        [[nodiscard]] size_t size() const noexcept
        {
            return entities.size();
        }

        void kill(entity_type e)
        {
            entity_store.push(e);
            entities[e] = snek::traits::tombstone_t<entity_type>::null_v;

            for (auto &s : _component_pools)
            {
                if (s)
                {
                    s->remove(e);
                }
            }
        }

        template <typename... Ts>
        [[nodiscard]] snek::ecs::light_view<this_type, Ts...> view()
        {
            static_assert((world_policy::template is_valid_component<Ts>() && ...), "Template pack must include world_policy components");
            return {_component_pools};
        }

        [[nodiscard]] allocator_type &
        get_allocator() const noexcept
        {
            return alloc;
        }
        ~world() {};
    };
}
