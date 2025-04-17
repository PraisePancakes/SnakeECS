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

    template <typename EntityT, typename ComponentList, typename AllocatorT = std::allocator<EntityT>>
    struct world_policy
    {
        using entity_type = EntityT;
        using component_list = ComponentList;
        using allocator_type = AllocatorT;

        static_assert(snek::entity::is_entity_type<EntityT>::value, "EntityT must meet following type requirements : uint64_t , uint32_t");
        static_assert(snek::traits::type_is_allocator<AllocatorT>::value, "AllocatorT must meet allocator requirements");
        static_assert(snek::traits::is_type_list<ComponentList>::value, "ComponentList must meet the component type list requirements");
        static_assert(component_list::size < component_list::list_size, "ComponentList size must be less than required list size criteria");

        template <typename C>
        [[nodiscard]] static constexpr size_t get_component_type_id()
        {

            return snek::utils::index_of<C, component_list>();
        }

        template <typename C>
        [[nodiscard]] static constexpr bool is_valid_component()
        {

            return (snek::utils::index_of<C, component_list>() != -1);
        }



        static inline entity_type generate_entity_id() noexcept
        {
            static entity_type new_id = 0;
            entity_type old = new_id;
            new_id++;
            return old;
        };
    };

    template <typename... Cs>
    using component_list = snek::utils::type_list<Cs...>;
}
