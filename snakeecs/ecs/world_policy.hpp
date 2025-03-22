#pragma once
#include "../utils/type_list.hpp"
#include "entity.hpp"
#include "../utils/traits/snek_traits.hpp"
namespace snek
{
    // config policy
    //@params
    // #ENTITY_TYPE
    // #COMPONENT_TYPE
    // #ALLOCATOR_TYPE

    template <typename EntityT, typename ComponentList, typename AllocatorT>
    struct world_policy;

    template <typename EntityT, typename ComponentList, typename AllocatorT>
    struct world_policy
    {
        using entity_index = EntityT;
        using component_list = ComponentList;
        using allocator_type = AllocatorT;

        static_assert(snek::entity::is_entity_type<EntityT>::value, "EntityT must meet following type requirements : uint64_t , uint32_t");
        static_assert(snek::traits::type_is_allocator<AllocatorT>::value, "AllocatorT must meet allocator requirements");

        template <typename C>
        [[nodiscard]] static constexpr size_t get_component_type_id()
        {

            return snek::utils::index_of<C, component_list>();
        };

        template <typename C>
        [[nodiscard]] static constexpr bool is_valid_component()
        {

            return (snek::utils::index_of<C, component_list>() != -1);
        };

        static inline auto generate_entity_id() noexcept
        {
            static EntityT new_id = 0;
            EntityT old = new_id;
            new_id++;
            return old;
        };
    };

    template <typename... Cs>
    using component_list = snek::utils::type_list<Cs...>;
}
