#pragma once
#include "../core/storage/sparse_set.hpp"
#include <functional>

namespace snek
{
    namespace ecs
    {

        /**
   * @summary:
   *    to better describe my intuition behind this view structure,
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

            // first_component is the basis for the pool lookups,
            // each entity of type "first_component" will refer to the entities of other types via get_component<T>(e)
            // where e is an entity and T is the component type to retrieve.
            /*

                +------------+----------+
                | Components | Entities |
                +------------+----------+
       -> first | A          |  0, 1, 2 |
                | B          |  0, 1, 2 |
                | C          |     0, 2 |
                +------------+----------+

            iterating through first we can use the set of entities to get from the dense index of the other sets
            */

            using first_component = typename std::tuple_element<0, std::tuple<Ts...>>::type;
            constexpr static auto driving_index = world_policy::template get_component_type_id<first_component>();

            template <typename T>
            T &get_from(entity_type e)
            {
                auto c_id = world_policy::template get_component_type_id<T>();
                auto *ss = static_cast<snek::storage::sparse_set<T> *>(_component_pools[c_id]);
                return ss->get_ref(e);
            }

            template <typename T>
            bool valid(entity_type id)
            {
                auto c_id = world_policy::template get_component_type_id<T>();
                auto *ss = static_cast<snek::storage::sparse_set<T> *>(_component_pools[c_id]);
                if (ss)
                {
                    return ss->contains(id);
                }
                std::cerr << "SNEK ABORT FOR COMPONENT " << typeid(T).name() << " : INVALID ENTITY " << std::to_string(id) << std::endl;
                std::abort();
            };

            template <typename... Cs>
            bool check(entity_type id)
            {
                return (valid<Cs>(id) && ...);
            }

        public:
            light_view(const std::vector<snek::storage::polymorphic_sparse_set *> &cp) : _component_pools(cp) {};

            void for_each(std::function<void(Ts &...)> f)
            {
                auto *driving_pool = static_cast<snek::storage::sparse_set<first_component> *>(_component_pools[driving_index]);
                if (driving_pool)
                {
                    for (entity_type e : driving_pool->get_dense())
                    {
                        if (check<Ts...>(e))
                        {
                            f(get_from<Ts>(e)...);
                        }
                    }
                }
            };

            ~light_view() {};
        };
    }
}