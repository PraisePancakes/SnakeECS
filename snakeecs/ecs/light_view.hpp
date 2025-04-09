#pragma once
#include "../core/storage/page_storage.hpp"
#include "../core/storage/sparse_set.hpp"
#include <functional>

namespace snek
{
    namespace ecs
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
   *  @tparam World - underlying world type in which we are inspecting, this allows for common shared types.
   *  @tparam Ts... - parameter pack of type of components we want to view
   */

        template <typename World, typename... Ts>
        class light_view
        {
            using world_policy = World::world_policy;
            using entity_type = world_policy::entity_index;
            using component_list = world_policy::component_list;
            using allocator_type = world_policy::allocator_type;
            std::vector<snek::storage::polymorphic_sparse_set *> _component_pools;
            static constexpr std::uint64_t _component_mask = (world_policy::template get_component_type_id<Ts>() | ...);
            using first_component = typename std::tuple_element<0, std::tuple<Ts...>>::type;

            template <typename T>
            T &get_component(entity_type e)
            {
                auto *ss = static_cast<snek::storage::sparse_set<T> *>(_component_pools[world_policy::template get_component_type_id<T>()]);
                return *ss->get(e);
            }

        public:
            light_view(const std::vector<snek::storage::polymorphic_sparse_set *> &cp) : _component_pools(cp) {};

            void for_each(std::function<void(Ts &...)> f)
            {
                constexpr auto driving_index = world_policy::template get_component_type_id<first_component>();
                auto *driving_pool = static_cast<snek::storage::sparse_set<first_component> *>(_component_pools[driving_index]);

                for (entity_type e : driving_pool->get_dense())
                {
                    f(get_component<Ts>(e)...);
                }
            };

            ~light_view() {};
        };
    }
}