#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <map>
#include "snek_alloc.hpp"
#include <algorithm>
#include "entity.hpp"
#include "component.hpp"
#include "../common_types.hpp"

namespace snek
{
    template <u64 n, typename Alloc = internal::snek_linear_allocator<Entity>>
    class World
    {
    public:
        using alloc_traits = std::allocator_traits<Alloc>;
        // Entity* will store pointer to constructed type from the allocator which holds the object in preallocated memory.
        using EntityTable = std::unordered_map<u64, typename alloc_traits::pointer>;
        // where u64 is component mask over set of components.
        using GroupTable = std::unordered_map<u64, EntityTable>;
        using ComponentStateTable = std::unordered_map<u64, std::unordered_map<u64, Component *>>;
        static constexpr u64 max_size = n;
        GroupTable groups;

    private:
        EntityTable entity_map; // all entities

        ComponentStateTable cmp_states;

        Alloc _alloc;

        bool running;
        alloc_traits::pointer _region;

        [[nodiscard]] typename alloc_traits::pointer create_entity()
        {
            try
            {
                typename alloc_traits::pointer p = _alloc.construct();
                if (!p)
                {
                    throw std::runtime_error("construction overflowed maximum allowed world storage [max = " + std::to_string(max_size) + "]");
                }
                entity_map.insert(std::pair<u64, typename alloc_traits::pointer>(p->GetID(), p));
                return p;
            }
            catch (std::exception &e)
            {
                std::cerr << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }
        }

    public:
        World()
            : running(true),
              _region(alloc_traits::allocate(_alloc, n)) {

              };

        World(const World &o)
            : running(true),
              _region(o.entity_region) {};

        World(World &&o)
            : _region(std::move(o._region)),
              running(true) {};

        inline void WorldPause() { running = false; };

        [[nodiscard]] alloc_traits::value_type &Spawn()
        {
            return *(create_entity());
        };
        [[nodiscard]] alloc_traits::pointer GetEntityByID(u64 id) const noexcept
        {
            if (entity_map.find(id) == entity_map.end())
                return nullptr;
            return entity_map.at(id);
        };

        template <typename C>
        [[nodiscard]] bool HasComponent(const Entity &e) const
        {
            static_assert(std::is_base_of_v<Component, C>, "Custom component must inherit from snek::core::Component");
            u64 id = uuid::GenerateComponentID<C>();
            return ((e.GetComponentMask() & id) == id);
        }
        template <typename T, typename U, typename... Args>
        [[nodiscard]] bool HasComponent(const Entity &e) const noexcept
        {
            return HasComponent<T>(e) && (HasComponent<U, Args>(e) && ...);
        };

        template <typename C, typename... Args>
        C &BindComponent(Entity &e, Args &&...args)
        {
            // ct guard
            static_assert(std::is_base_of_v<Component, C>, "Custom component must inherit from snek::core::Component");

            // remove from previous group
            u64 old_mask = e.GetComponentMask();
            if (this->groups.find(old_mask) != this->groups.end())
            {
                EntityTable &entt_map = this->groups.at(old_mask);
                auto it = entt_map.find(e.GetID());
                if (it != entt_map.end())
                {
                    entt_map.erase(it);
                }
                if (entt_map.size() <= 0)
                {
                    auto it = this->groups.find(old_mask);
                    this->groups.erase(it);
                };
            }
            // set new component mask
            u64 id = uuid::GenerateComponentID<C>();
            e.SetComponentFlag(id);
            // construct component
            C *c = new C(std::forward<Args>(args)...);
            // bind it to entity
            c->owner = &e;
            // track it
            // --add to new group
            u64 new_mask = e.GetComponentMask();
            groups[new_mask][e.GetID()] = &e;
            // add to component state table
            cmp_states[e.GetID()][id] = c;
            return *c;
        };

        template <typename C>
        C *GetComponent(const Entity &e) noexcept
        {
            if (!HasComponent<C>(e))
            {
                return nullptr;
            };
            u64 c_id = uuid::GenerateComponentID<C>();
            return static_cast<C *>(cmp_states[e.GetID()][c_id]);
        };

        template <typename... Components>
        EntityTable &GetGroupView()
        {
            static_assert((std::is_base_of_v<Component, Components> && ...),
                          "All components must inherit from snek::core::Component");

            u64 mask = (uuid::GenerateComponentID<Components>() | ...);
            if (groups.find(mask) == groups.end())
            {
                static EntityTable empty_table;
                return empty_table;
            }

            return groups[mask];
        }

        [[nodiscard]] Alloc &GetWorldAllocator() const noexcept { return _alloc; };
        // convert to variadic template to check if all entity ids exist, ensure argument set shares the same type
        [[nodiscard]] bool HasEntity(const alloc_traits::value_type &e) const noexcept
        {
            return entity_map.find(e.GetID()) != entity_map.end();
        };
        [[nodiscard]] bool HasEntity(u64 id) const noexcept
        {
            return entity_map.find(id) != entity_map.end();
        };
        [[nodiscard]] bool IsRunning() const
        {
            return this->running;
        }
        ~World() {

        };
        // add entity
        // remove entity
        // add_entity(tag)
        // get_entity(id)
        // has_entity(id)
        // get_entities(key) -> vector
    };
};