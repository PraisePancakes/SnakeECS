#pragma once
#include "../core/storage/sparse_set.hpp"
#include <functional>

namespace snek::ecs
{
    /**
* @summary:
*    to better describe my intuition behind this view structure,
*  lets understand the definition of what it means to view something.
*   a view, in the context of an ECS, refers to, but does not own, a set of entities
*
   In the case of a world we may want to, view a set of entities to inspect them, transform their underlying components.
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
    namespace internal
    {
        template <typename WorldPolicy, typename T>
        bool helper_matched_ids(size_t id)
        {
            return WorldPolicy::template get_component_type_id<T>() == id;
        }

        template <typename WorldPolicy, typename... Cs>
        bool matched_ids(size_t id)
        {
            return (helper_matched_ids<WorldPolicy, Cs>(id) || ...);
        }
    }

    using namespace internal;
    template <typename World, typename... Ts>
    class light_view
    {
        using world_policy = World::world_policy;
        using entity_type = world_policy::entity_type;
        using component_list = world_policy::component_list;
        using allocator_type = world_policy::allocator_type;
        std::vector<snek::storage::polymorphic_sparse_set *> _component_pools;
        std::vector<std::vector<size_t>> _filtered;

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
            return (ss && ss->contains(id));
        }

        template <typename... Cs>
        bool check(entity_type id)
        {
            return (valid<Cs>(id) && ...);
        }

        template <typename WorldPolicy>
        class sparse_view_iterator
        {
            using world_policy = WorldPolicy;
            std::vector<std::vector<size_t>> filtered;
            size_t row;
            size_t col;

        public:
            using value_type = world_policy::entity_type;
            using pointer = value_type *;
            using difference_type = std::ptrdiff_t;
            using size_type = size_t;
            using iterator_category = std::bidirectional_iterator_tag;
            using reference = value_type &;
            using const_pointer = const value_type *;
            using const_reference = const value_type &;

            sparse_view_iterator(const std::vector<std::vector<size_t>> &filtered, size_t row, size_t col)
                : filtered(filtered),
                  row{row},
                  col{col} {};

            sparse_view_iterator &operator++()
            {
                if (col < filtered[row].size())
                {
                    col++;
                }

                if (row < filtered.size() && col >= filtered[row].size())
                {
                    row++;
                    col = 0;
                }
                return *this;
            }

            sparse_view_iterator &operator--()
            {
                if (col > 0)
                {
                    col--;
                }
                else
                {
                    if (row > 0 && col <= 0)
                    {
                        row--;
                        col = filtered[row].size() - 1;
                    }
                }

                return *this;
            };

            reference operator*()
            {
                return filtered[row][col];
            };

            const_reference operator->()
            {
                return filtered[row][col];
            };

            bool operator==(const sparse_view_iterator &o) const
            {
                return (row == o.row && col == o.col);
            }
            bool operator!=(const sparse_view_iterator &o) const
            {
                return !(*this == o);
            }
            ~sparse_view_iterator() {};
        };

        template <typename WorldPolicy>
        class const_sparse_view_iterator
        {
            using world_policy = WorldPolicy;
            std::vector<std::vector<size_t>> filtered;
            size_t row;
            size_t col;

        public:
            using value_type = world_policy::entity_type;
            using pointer = value_type *;
            using difference_type = std::ptrdiff_t;
            using size_type = size_t;
            using iterator_category = std::bidirectional_iterator_tag;
            using reference = value_type &;
            using const_pointer = const value_type *;
            using const_reference = const value_type &;

            const_sparse_view_iterator(const std::vector<std::vector<size_t>> &filtered, size_t row, size_t col)
                : filtered(filtered),
                  row{row},
                  col{col} {};

            const_sparse_view_iterator &operator++()
            {
                if (col < filtered[row].size())
                {
                    col++;
                }

                if (row < filtered.size() && col >= filtered[row].size())
                {
                    row++;
                    col = 0;
                }
                return *this;
            }

            const_sparse_view_iterator &operator--()
            {
                if (col > 0)
                {
                    col--;
                }
                else
                {
                    if (row > 0 && col <= 0)
                    {
                        row--;
                        col = filtered[row].size() - 1;
                    }
                }

                return *this;
            };

            const_reference operator*() const
            {
                return filtered[row][col];
            };

            const_reference operator->() const
            {
                return filtered[row][col];
            };

            bool operator==(const const_sparse_view_iterator &o) const
            {
                return (row == o.row && col == o.col);
            }
            bool operator!=(const const_sparse_view_iterator &o) const
            {
                return !(*this == o);
            }
            ~const_sparse_view_iterator() {};
        };

    public:
        using iterator = sparse_view_iterator<world_policy>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_iterator = const_sparse_view_iterator<world_policy>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        light_view(const std::vector<snek::storage::polymorphic_sparse_set *> &cp) : _component_pools(cp)
        {
            for (size_t i = 0; i < cp.size(); i++)
            {
                if (cp[i] && matched_ids<world_policy, Ts...>(i))
                {
                    _filtered.push_back(cp[i]->get_dense());
                }
            }
        };

        iterator begin()
        {
            return iterator(_filtered, 0, 0);
        };

        iterator end()
        {
            int end_row = _filtered.size();
            int end_col = 0;
            return iterator(_filtered, end_row, end_col);
        }

        reverse_iterator rbegin()
        {
            int end_row = _filtered.size();
            int end_col = 0;
            iterator it(_filtered, end_row, end_col);
            return reverse_iterator(it);
        }

        reverse_iterator rend()
        {
            iterator it(_filtered, 0, 0);
            return reverse_iterator(it);
        }

        const_iterator cbegin()
        {
            return const_iterator(_filtered, 0, 0);
        };

        const_iterator cend()
        {
            int end_row = _filtered.size();
            int end_col = 0;
            return const_iterator(_filtered, end_row, end_col);
        }

        template <typename T>
        T &get(const entity_type e)
        {
            // down cast here is guarenteed to be well-defined, id known at compile time, pools assorted via id.
            return static_cast<snek::storage::sparse_set<T> *>(_component_pools[world_policy::template get_component_type_id<T>()])->get_ref(e);
        }

        void for_each(std::function<void(Ts &...)> f)
        {
            auto *driving_pool = static_cast<snek::storage::sparse_set<first_component> *>(_component_pools[driving_index]);
            if (driving_pool)
            {
                for (auto e : driving_pool->get_dense())
                {
                    if (check<Ts...>(e))
                    {
                        f(get_from<Ts>(e)...);
                    }
                }
            }
        };

        void for_each(std::function<void(entity_type id, Ts &...)> f)
        {
            auto *driving_pool = static_cast<snek::storage::sparse_set<first_component> *>(_component_pools[driving_index]);
            if (driving_pool)
            {
                for (auto e : driving_pool->get_dense())
                {
                    if (check<Ts...>(e))
                    {
                        f(e, get_from<Ts>(e)...);
                    }
                }
            }
        };

        ~light_view() {};
    };

}