#pragma once
#include "../utils/type_list.hpp"
#include "entity.hpp"
#include "../utils/traits/snek_traits.hpp"
#include "traits.hpp"
#include <limits>

namespace snek
{
    // config policy
    //@params
    // #ENTITY_TYPE
    // #COMPONENT_TYPE
    // #ALLOCATOR_TYPE
    using namespace traits;

    namespace helper
    {
        template <typename Policy, typename C>
        [[nodiscard]] static constexpr bool check_valid_component_index()
        {
            return (snek::utils::index_of<std::remove_cv_t<C>, typename Policy::component_list>() != -1);
        }

    }

    template <typename EntityT, typename ComponentList, typename AllocatorT, typename TagT>
    struct world_policy;

    template <typename EntityT, typename ComponentList, typename AllocatorT = std::allocator<EntityT>, typename TagT>
    struct world_policy
    {

        using this_type = world_policy<EntityT, ComponentList, AllocatorT, TagT>;
        using component_list = ComponentList;
        using allocator_type = AllocatorT;
        using tag_type = TagT;
        using traits = entity::entity_traits<EntityT>;

        using entity_type = traits::entity_type;
        using version_type = traits::version_type;
        static constexpr auto version_size = std::numeric_limits<version_type>::digits;
        static constexpr auto max_version = std::numeric_limits<version_type>::max();
        static constexpr auto tombstone_v = snek::traits::tombstone_t<entity_type>::null_v;

        static_assert(std::is_enum_v<TagT> || std::is_integral_v<TagT>, "TagT must be an enumurable or enum type");
        static_assert(snek::entity::is_entity_type<EntityT>::value, "EntityT must meet following type requirements : uint64_t , uint32_t");
        static_assert(snek::traits::type_is_allocator<AllocatorT>::value, "AllocatorT must meet allocator requirements");
        static_assert(snek::traits::is_type_list<ComponentList>::value, "ComponentList must meet the component type list requirements");
        static_assert(component_list::size <= component_list::list_size, "ComponentList size must be less than required list size criteria");

        template <typename C>
        [[nodiscard]] static constexpr size_t get_component_type_id()
        {

            return snek::utils::index_of<C, component_list>();
        }

        template <typename Cs>
        [[nodiscard]] static constexpr bool is_valid_component()
        {
            return (helper::check_valid_component_index<this_type, Cs>());
        }

        template <typename... Cs>
        [[nodiscard]] static constexpr bool is_valid_component_set()
        {
            return (helper::check_valid_component_index<this_type, Cs>() && ...);
        }

        [[nodiscard]] static entity_type to_entity(entity_type id)
        {
            // get higher bit representation of entity
            return (id >> version_size);
        };

        [[nodiscard]] static entity_type to_version(entity_type id)
        {
            // get lower bit representation of version
            return (id & traits::version_mask);
        }

        [[nodiscard]] static bool is_tombstone(const entity_type id)
        {
            return to_entity(id) == to_entity(tombstone_v);
        }

        static void to_tombstone(entity_type &id)
        {
            auto lo = to_version(id);
            auto hi = to_entity(id);
            hi |= tombstone_v;
            id = (hi << version_size | (lo));
        };

        static void increment_version(entity_type &id)
        {
            if (to_version(id) >= max_version)
                return;
            auto lo = to_version(id);
            auto hi = to_entity(id);

            id = (hi << version_size | (++lo));
        };

        static inline entity_type generate_entity_id() noexcept
        {
            static entity_type new_id = 0;
            entity_type old = new_id;
            new_id++;
            return (old <<= version_size);
        };
    };

    template <typename... Cs>
    using component_list = snek::utils::type_list<Cs...>;
}
