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
        using ComponentTable = std::unordered_map<u64, EntityTable>;

    private:
        EntityTable entity_map;
        // where u64 is component mask over set of components.
        ComponentTable groups;
        Alloc _alloc;
        static constexpr u64 max_size = n;
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
        [[nodiscard]] bool HasComponent(const Entity &e)
        {
            static_assert(std::is_base_of_v<Component, C>, "Custom component must inherit from snek::core::Component");
            u64 id = uuid::GenerateComponentID<C>();
            return ((e.GetComponentMask() & id) == id);
        }
        template <typename T, typename U, typename... Args>
        [[nodiscard]] bool HasComponent(const Entity &e) const noexcept
        {
            return (HasComponent<T>() && HasComponent<U>() && (HasComponent<Args>() && ...));
        };
        template <typename C, typename... Args>
        C &BindComponent(Entity &e, Args &&...args)
        {
            // ct guard
            static_assert(std::is_base_of_v<Component, C>, "Custom component must inherit from snek::core::Component");

            // remove from previous group
            u64 old_mask = e.GetComponentMask();
            groups[old_mask][e.GetID()] = nullptr;

            e.SetComponentFlag(uuid::GenerateComponentID<C>());
            // construct component
            C *c = new C(std::forward<Args>(args)...);
            // bind it to entity
            c->owner = &e;
            // track it
            // --add to new group
            u64 new_mask = e.GetComponentMask();
            groups[new_mask][e.GetID()] = &e;
            return *c;
        };

        template <typename C>
        C *GetComponent(const Entity &e)
        {
            if (!HasComponent<C>(e))
            {
                return nullptr;
            };
            

        };

        template <typename C, typename... Cs>
        EntityTable EntitiesWithComponents() {

        };

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