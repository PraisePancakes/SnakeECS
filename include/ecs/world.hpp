#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <map>
#include "entity_allocator.hpp"
#include <algorithm>
#include "entity.hpp"
#include "../common_types.hpp"
#include <array>
#include "view.hpp"

#define SNEK_ASSERT(value, msg) \
    static_assert(value, msg)

namespace snek
{

    template <entity Size, typename Alloc>
    class basic_world
    {

    public:
        using alloc_traits = std::allocator_traits<Alloc>;
        using value_type = typename alloc_traits::value_type;

        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename alloc_traits::pointer;
        static constexpr entity max_size = Size;

    protected:
        [[nodiscard]] pointer create_entity()
        {
            try
            {
                u64 id = uuid::generate_entity_id();
                pointer p = std::construct_at(entity_pool + id, id);
                if (!p)
                {
                    throw std::runtime_error("construction overflowed maximum allowed world storage [max = " + std::to_string(max_size) + "]");
                }

                return p;
            }
            catch (std::exception &e)
            {
                std::cerr << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }
        }

    public:
        pointer entity_pool;
        Alloc _alloc;

        basic_world() : entity_pool(alloc_traits::allocate(_alloc, Size)) {
                        };
        basic_world(const basic_world &o)
            : entity_pool(o.entity_region) {};

        basic_world(basic_world &&o)
            : entity_pool(std::move(o._region)) {};

        virtual ~basic_world()
        {
            if (this->entity_pool)
            {
                _alloc.deallocate(entity_pool, Size);
            }
        };
    };

    template <entity Size, typename Alloc = snek::allocator::entity_allocator<entity>>
    class world : public basic_world<Size, Alloc>
    {
    public:
        using underlying_type = world<Size, Alloc>;
        using alloc_traits = std::allocator_traits<Alloc>;
        using value_type = typename alloc_traits::value_type;

        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename alloc_traits::pointer;

        // Entity* will store pointer to constructed type from the allocator which holds the object in preallocated memory.
        using entity_array = std::array<pointer, Size>;
        using component_array = std::array<void *, Size>;
        using component_mask_array = std::array<component_mask, Size>;
        // where u64 is component mask over set of components.
        using group_table = std::unordered_map<component_mask, entity_array>;
        using component_state_table = std::unordered_map<component_id, component_array>;

    private:
        group_table groups;
        component_state_table cmp_states;
        component_mask_array masks;
        size_t _active;
        bool _running;

        // region allocated by internal allocator, this region holds a pointer to each entity initialized within that region,
        // the containers in this class will query from this preallocated region and store pointers to those entities.

    public:
        world()
            : groups{}, cmp_states{}, masks{}, _active{}, _running(true) {};

        world(const world &o)
            : groups{o.groups}, cmp_states{o.cmp_states}, masks{o.masks}, _active{o._active}, _running(true) {};

        world(world &&o)
            : groups{std::move(o.groups)}, cmp_states{std::move(o.cmp_states)}, masks{std::move(o.masks)}, _active{o._active}, _running(true) {};

        inline void pause() { _running = false; };

        [[nodiscard]] alloc_traits::value_type &spawn()
        {
            ++_active;
            return *this->create_entity();
        };

        [[nodiscard]] pointer get_entity_by_id(const u64 id) const
        {
            return &this->entity_pool[id];
        }

        template <typename C>
        [[nodiscard]] bool has(const_reference e) const
        {
            SNEK_ASSERT(std::is_class_v<C>, "Component must succeed std::is_class_v<T> type trait");
            u64 id = uuid::generate_component_id<C>();
            return ((masks[e] & id) == id);
        }

        template <typename T, typename U, typename... Args>
        [[nodiscard]] bool has_any(const_reference e) const noexcept
        {
            return has<T>(e) || has<U>(e) || (has<Args>(e) || ...);
        }

        template <typename T, typename U, typename... Args>
        [[nodiscard]] bool has_all(const_reference e) const noexcept
        {
            return has<T>(e) && (has<U, Args>(e) && ...);
        };

        template <typename C, typename... Args>
        C &bind(reference e, Args &&...args)
        {
            // ct guard
            SNEK_ASSERT(std::is_class_v<C>, "Component must succeed std::is_class_v<T> type trait");

            // remove from previous group
            u64 old_mask = masks[e];
            if (groups.size() > 0 && this->groups.find(old_mask) != this->groups.end())
            {
                entity_array &archetype_array = this->groups.at(old_mask);
                auto it = archetype_array.at(e);
                if (it != archetype_array.back())
                {
                    archetype_array[e] = nullptr;
                }
            }
            // set new component mask
            u64 id = uuid::generate_component_id<C>();
            masks[e] |= id;
            // construct component
            C *c = new C(std::forward<Args>(args)...);
            // track it
            // --add to new group
            u64 new_mask = masks[e];
            groups[new_mask][e] = &this->entity_pool[e];
            // add to component state table
            cmp_states[id][e] = c;

            return *c;
        };

        template <typename T>
        void remove_component(reference e) noexcept
        {
            if (!has<T>(e))
                return;
            SNEK_ASSERT(std::is_class_v<T>, "Component must succeed std::is_class_v<T> type trait");
            u64 id = (uuid::generate_component_id<T>());
            u64 old_mask = masks[e];
            if (groups.size() > 0 && this->groups.find(old_mask) != this->groups.end())
            {
                entity_array &archetype_array = this->groups.at(old_mask);
                auto it = archetype_array.at(e);
                if (it != archetype_array.back())
                {
                    archetype_array[e] = nullptr;
                }
            }
            masks[e] &= ~(id);
            u64 new_mask = masks[e];
            groups[new_mask][e] = &this->entity_pool[e];
            cmp_states[id][e] = nullptr;
        };

        template <typename T, typename U, typename... Args>
        void remove_component(reference e) noexcept
        {
            remove_component<T>();
            remove_component<U>();
            (remove_component<Args>(), ...);
        };

        [[nodiscard]] bool contains(const_reference e) const
        {
            return this->entity_pool[e];
        }

            void kill(reference e)
        {
            // remove from current group;
            this->groups[masks[e]][e] = nullptr;
            // remove all its component states
            for (auto &c : cmp_states)
            {
                c[e] = nullptr;
            }
            // remove from pool
            std::destroy_at(this->entity_pool + e);
        }

        size_t size() const
        {
            return _active;
        };

        constexpr u64 capacity()
        {
            return this->max_size;
        };

        template <typename... Cs>
        snek::light_view<underlying_type, Cs...> view()
        {
            snek::light_view<underlying_type, Cs...> v(cmp_states, groups);
            return v;
        };

        template <typename... T, typename... Args>
        void initialize(reference e, Args &&...args)
        {
            (bind<T>(e, std::forward<Args>(args)), ...);
        }

        template <typename C>
        C *get_component(const_reference e) noexcept
        {
            if (!has<C>(e))
            {
                return nullptr;
            };

            u64 c_id = uuid::generate_component_id<C>();
            return static_cast<C *>(cmp_states[c_id][e]);
        };

        [[nodiscard]] Alloc &get_allocator() const noexcept { return this->_alloc; };

        [[nodiscard]] bool running() const
        {
            return this->_running;
        }
        ~world()
        {
            _active = 0;
            _running = false;
        };
    };
};