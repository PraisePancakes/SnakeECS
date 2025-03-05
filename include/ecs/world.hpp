#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <map>
#include "verbose_allocator.hpp"
#include <algorithm>
#include "entity.hpp"
#include "component.hpp"
#include "../common_types.hpp"
#include <array>
#include "view.hpp"

namespace snek
{
    template <u64 n, typename Alloc = snek::allocator::verbose_allocator<Entity>>
    class World
    {
    public:
        using alloc_traits = std::allocator_traits<Alloc>;
        using EntityID = u64;
        using ComponentID = u64;
        using ComponentMask = u64;
        // Entity* will store pointer to constructed type from the allocator which holds the object in preallocated memory.
        using EntityArray = std::array<typename alloc_traits::pointer, n>;
        using ComponentArray = std::array<Component *, n>;
        // where u64 is component mask over set of components.
        using GroupTable = std::unordered_map<ComponentMask, EntityArray>;
        using ComponentStateTable = std::unordered_map<ComponentID, ComponentArray>;
        static constexpr EntityID max_size = n;

    private:
        EntityArray entity_array; // all entities
        GroupTable groups;
        ComponentStateTable cmp_states;

        Alloc _alloc;

        bool running;

        // region allocated by internal allocator, this region holds a pointer to each entity initialized within that region,
        // the containers in this class will query from this preallocated region and store pointers to those entities.
        alloc_traits::pointer _region;

        [[nodiscard]] typename alloc_traits::pointer create_entity()
        {
            try
            {
                u64 id = uuid::GenerateEntityID();
                typename alloc_traits::pointer p = std::construct_at(_region + id);
                if (!p)
                {
                    throw std::runtime_error("construction overflowed maximum allowed world storage [max = " + std::to_string(max_size) + "]");
                }
                p->SetID(id);
                entity_array[id] = p;
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
            return entity_array[id];
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
                EntityArray &entt_arr = this->groups.at(old_mask);
                auto it = entt_arr.at(e.GetID());
                if (it != entt_arr.back())
                {
                    entt_arr[e.GetID()] = nullptr;
                }
                if (entt_arr.size() <= 0)
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
            cmp_states[id][e.GetID()] = c;
            /*  change cmp_states to be a 2d array of components as row and entity (by index) as column

                c1  0, - , 2, -, -, 5
                c2  ...
                c3  ...



            */
            return *c;
        };

        template <typename... Cs>
        snek::light_view<Cs...> view() {};

        template <typename... T, typename... Args>
        void InitializeComponents(Entity &e, Args &&...args)
        {
            (BindComponent<T>(e, std::forward<Args>(args)), ...);
        }

        template <typename C>
        C *GetComponent(const Entity &e) noexcept
        {
            if (!HasComponent<C>(e))
            {
                return nullptr;
            };
            u64 c_id = uuid::GenerateComponentID<C>();
            return static_cast<C *>(cmp_states[c_id][e.GetID()]);
        };

        template <typename... Components>
        EntityArray GetGroupView()
        {
            static_assert((std::is_base_of_v<Component, Components> && ...),
                          "All components must inherit from snek::core::Component");

            u64 mask = (uuid::GenerateComponentID<Components>() | ...);
            if (groups.find(mask) == groups.end())
            {
                static EntityArray empty_table;
                return empty_table;
            }
            return groups[mask];
        }

        [[nodiscard]] Alloc &GetWorldAllocator() const noexcept { return _alloc; };
        // convert to variadic template to check if all entity ids exist, ensure argument set shares the same type
        [[nodiscard]] bool HasEntity(const alloc_traits::value_type &e) const noexcept
        {
            return entity_array.find(e.GetID()) != entity_array.end();
        };
        [[nodiscard]] bool HasEntity(u64 id) const noexcept
        {
            return entity_array.find(id) != entity_array.end();
        };
        [[nodiscard]] bool IsRunning() const
        {
            return this->running;
        }
        ~World() {

        };
    };
};