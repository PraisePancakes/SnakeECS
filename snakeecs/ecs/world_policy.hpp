#pragma once
#include "../utils/type_list.hpp"
#include "entity.hpp"

namespace snek
{

    template <typename EntityT, typename ComponentList>
    struct world_policy;

    template <typename EntityT, typename ComponentList>
    struct world_policy
    {
        using entity_index = EntityT;
        using component_list = ComponentList;
        static_assert(snek::entity::is_entity_type<EntityT>::value, "invalid entity type, entities can be of the following types : uint64_t , uint32_t");

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
