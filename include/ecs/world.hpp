#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <map>
#include "verbose_allocator.hpp"
#include <algorithm>
#include "entity.hpp"
#include "../common_types.hpp"
#include <array>
#include "view.hpp"

namespace snek
{
    template <u64 Size, typename Alloc = snek::allocator::verbose_allocator<Entity>>
    class World
    {
    public:
        using alloc_traits = std::allocator_traits<Alloc>;
        using pointer = typename alloc_traits::pointer;
        using EntityID = u64;
        using ComponentID = u64;
        using ComponentMask = u64;
        // Entity* will store pointer to constructed type from the allocator which holds the object in preallocated memory.
        using EntityArray = std::array<pointer, Size>;
        using ComponentArray = std::array<void *, Size>;
        // where u64 is component mask over set of components.
        using GroupTable = std::unordered_map<ComponentMask, EntityArray>;
        using ComponentStateTable = std::unordered_map<ComponentID, ComponentArray>;
        static constexpr EntityID max_size = Size;

    private:
        GroupTable groups;
        ComponentStateTable cmp_states;
        pointer entity_pool;
        Alloc _alloc;

        bool running;

        // region allocated by internal allocator, this region holds a pointer to each entity initialized within that region,
        // the containers in this class will query from this preallocated region and store pointers to those entities.

        [[nodiscard]] pointer create_entity()
        {
            try
            {
                u64 id = uuid::GenerateEntityID();
                pointer p = std::construct_at(entity_pool + id);
                if (!p)
                {
                    throw std::runtime_error("construction overflowed maximum allowed world storage [max = " + std::to_string(max_size) + "]");
                }
                p->SetID(id);
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
              entity_pool(alloc_traits::allocate(_alloc, Size)) {

              };

        World(const World &o)
            : running(true),
              entity_pool(o.entity_region) {};

        World(World &&o)
            : entity_pool(std::move(o._region)),
              running(true) {};

        inline void WorldPause() { running = false; };

        [[nodiscard]] alloc_traits::value_type &Spawn()
        {
            return *(create_entity());
        };

        [[nodiscard]] snek::Entity *GetEntityById(const u64 id) const
        {
            return &entity_pool[id];
        }

        template <typename C>
        [[nodiscard]] bool HasComponent(const Entity &e) const
        {
            static_assert(std::is_class_v<C>, "Component must pass class type trait");
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
            static_assert(std::is_class_v<C>, "Component must pass class type trait");

            // remove from previous group
            u64 old_mask = e.GetComponentMask();
            if (groups.size() > 0 && this->groups.find(old_mask) != this->groups.end())
            {
                EntityArray &archetype_array = this->groups.at(old_mask);
                auto it = archetype_array.at(e.GetID());
                if (it != archetype_array.back())
                {
                    archetype_array[e.GetID()] = nullptr;
                }
                if (archetype_array.size() <= 0)
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
            // track it
            // --add to new group
            u64 new_mask = e.GetComponentMask();
            groups[new_mask][e.GetID()] = &entity_pool[e.GetID()];
            // add to component state table
            cmp_states[id][e.GetID()] = c;

            return *c;
        };

        constexpr u64 size()
        {
            return max_size;
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
            static_assert((std::is_class_v<Components> && ...),
                          "Component must pass class type trait");
            u64 mask = (uuid::GenerateComponentID<Components>() | ...);
            if (groups.find(mask) == groups.end())
            {
                static EntityArray empty_table;
                return empty_table;
            }
            return groups[mask];
        }

        [[nodiscard]] Alloc &GetWorldAllocator() const noexcept { return _alloc; };

        [[nodiscard]] bool IsRunning() const
        {
            return this->running;
        }
        ~World() {

        };
    };
};