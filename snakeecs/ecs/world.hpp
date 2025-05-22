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
        using version_type = Policy::version_type;
        using component_list = Policy::component_list;
        using allocator_type = Policy::allocator_type;
        using tag_type = Policy::tag_type;
        using underlying_tag_type = std::underlying_type_t<tag_type>;

    private:
        using this_type = world<Policy>;

        std::queue<entity_type> entity_store;
        std::vector<entity_type, allocator_type> entities;
        std::vector<snek::storage::polymorphic_sparse_set *> _component_pools;
        std::unordered_map<underlying_tag_type, snek::storage::sparse_set<entity_type>> _tagged_entities;

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
            bool overflowed = (world_policy::to_entity(id) >= snek::traits::tombstone_t<entity_type>::null_v);

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
            _tagged_entities[-1].insert(world_policy::to_entity(id), world_policy::to_entity(id));

            return world_policy::to_entity(id);
        };

        [[nodiscard]] entity_type spawn(tag_type tag)
        {
            return spawn(static_cast<underlying_tag_type>(tag));
        }

        [[nodiscard]] entity_type spawn(underlying_tag_type tag)
        {
            entity_type id = world_policy::generate_entity_id();
            bool overflowed = (world_policy::to_entity(id) >= snek::traits::tombstone_t<entity_type>::null_v);
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
            _tagged_entities[tag].insert(world_policy::to_entity(id), world_policy::to_entity(id));

            return world_policy::to_entity(id); // return index of entity
        };

        [[nodiscard]] bool contains(entity_type id) const
        {

            return id < entities.size() &&
                   !world_policy::is_tombstone(entities[id]);
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

        [[nodiscard]] entity_type to_version(const entity_type index)
        {
            return world_policy::to_version(entities[index]);
        }

        [[nodiscard]] entity_type to_id(const entity_type index)
        {
            return world_policy::to_entity(entities[index]);
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

        std::vector<entity_type> get_tagged_entities(tag_type tag)
        {
            return get_tagged_entities(static_cast<underlying_tag_type>(tag));
        }

        std::vector<entity_type> get_tagged_entities(underlying_tag_type tag)
        {
            return std::vector<entity_type>(this->_tagged_entities[tag].begin(), this->_tagged_entities[tag].end());
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
        C *get(entity_type e)
        {
            SNEK_ASSERT(world_policy::template is_valid_component<C>(), "C must be a registered component. ");
            size_t c_id = world_policy::template get_component_type_id<C>();
            auto ss = static_cast<snek::storage::sparse_set<C> *>(_component_pools[c_id]);
            return ss->get(e);
        }

        template <typename C>
        C &get_ref(entity_type e)
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
            for (auto &ss : _tagged_entities)
            {
                if (ss.second.contains(e))
                {
                    ss.second.remove(e);
                }
            }

            world_policy::increment_version(entities[e]);
            entity_store.push(entities[e]);
            world_policy::to_tombstone(entities[e]); // converts entity bits to tombstone, leaves version untouched

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
            static_assert((world_policy::template is_valid_component_set<Ts...>()), "Template pack must include world_policy components");
            return {_component_pools};
        }

        template <typename... Ts>
        [[nodiscard]] const snek::ecs::light_view<this_type, Ts...> view() const
        {
            static_assert((world_policy::template is_valid_component_set<Ts...>()), "Template pack must include world_policy components");
            return {_component_pools};
        }

        [[nodiscard]] allocator_type &
        get_allocator() const noexcept
        {
            return alloc;
        }
        ~world() = default;
    };
}
