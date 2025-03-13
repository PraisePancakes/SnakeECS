#pragma once
#include <tuple>
#include "../utils/identifier.hpp"
#include <functional>
#include <type_traits>
#include <array>
namespace snek
{

    /**
    * @summary:
    *    to better describe my intuition behind my intuition behind this view structure,
    *  lets understand the definition of what it means to view something.
    *  view
       /vyo͞o/
       verb
           1. look at or inspect (something).

        In terms of entity-component systems, in the case of a world of entities we may want to, view a set of entities to inspect them, modify their underlying components.
        we can denote the view of entities as A, and the pool of entities as B.
        where A ⊂ B. in the proper subset A, there exists a group of entities that have similar components, it would be much more efficient to operate on these groups as a whole rather than individually.
        say we have a pool of entities :
        [E0 : {A& , B&}, E1 : {A&, B&},  E2 : {A&, B&, C&}, E3 : {D& , F&}]
        we can pull a "view" of entities that at least both component A and component B
        V : [E0 : {A& , B&}, E1 : {A&, B&},  E2 : {A&, B&, C&}]
        now with this extraction of entities we can iterate and operate on each component via some lamba expression
        V operate on similar entities via lambda[&](A& a, B& b) {
            //do something with a stored in current entity
            //do something with b stored in current entity
        }

       @class:
           - a light_view simply encapsulates a group of functionalities that can be performed on a view.
    *
    *
    *  @tparam WorldType - underlying world type in which we are inspecting, this allows for common shared types.
    *  @tparam Components - parameter pack of type of components we want to view
    */

    template <typename WorldType, typename... Components>
    class light_view
    {

        using underlying_type = WorldType;
        //  auto view = world.view<T, A>();
        using entity_array = WorldType::entity_array;
        using component_array = WorldType::component_array;
        // where u64 is component mask over set of components.
        using group_table = WorldType::group_table;
        using component_state_table = WorldType::component_state_table;

        using alloc_traits = WorldType::alloc_traits;
        using value_type = WorldType::value_type;
        using reference = WorldType::reference;
        using const_reference = WorldType::const_reference;
        using pointer = WorldType::pointer;

        static_assert((std::is_class_v<Components> && ...),
                      "Component must pass class type trait");

        component_state_table _state_table;
        group_table _groups;

    public:
        light_view(component_state_table &st, group_table &g) : _state_table(st), _groups(g) {};

        void for_each(std::function<void(Components &...)> f)
        {
            component_mask cmp_mask = (uuid::generate_component_id<Components>() | ...);
            auto archetypes = _groups[cmp_mask];
            for (const auto &e : archetypes)
            {
                if (!e)
                    continue;
                f(*static_cast<Components *>(_state_table[uuid::generate_component_id<Components>()][*e])...);
            }
        };

        ~light_view() {};
    };

}