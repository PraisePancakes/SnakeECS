#pragma once
#include "../utils/utils.hpp"

namespace snek
{

    template <typename EntityT, typename ComponentList>
    struct world_policy;

    template <typename EntityT, typename ComponentList>
    struct world_policy
    {
        using entity_index_t = EntityT;
        using component_list = ComponentList;

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
    };
    
    template <typename... Cs>
    using component_list = snek::utils::type_list<Cs...>;
}
