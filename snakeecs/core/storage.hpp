#pragma once
#include "../debug/debug.hpp"
#include <iostream>
#include <vector>
#include "../utils/type_descriptor.hpp"
#include "../ecs/traits.hpp"
#include <memory>

namespace snek
{
    namespace storage
    {
#define PAGE_SIZE 4
        template <typename T, typename Alloc>
        class page_storage
        {
            typedef std::array<T, PAGE_SIZE> page_array_t;
            typedef std::allocator_traits<Alloc>::template rebind_alloc<page_array_t> internal_allocator;
            std::vector<page_array_t, internal_allocator> pages;

            static constexpr auto tombstone_v = snek::traits::tombstone_t<T>::null_v;
            void resize(size_t new_size)
            {
                std::array<T, PAGE_SIZE> arr;
                arr.fill(tombstone_v);
                pages.resize(new_size, arr);
            }

        public:
            explicit page_storage(size_t size)
            {
                resize(size);
#if _SNEK_DEBUG_
                LOG("basic storage T : " + snek::utils::type_descriptor<T>().stringify());
#endif
            };

            explicit page_storage()
            {
                resize(10);
#if _SNEK_DEBUG_
                LOG("basic storage T : " + snek::utils::type_descriptor<T>().stringify());
#endif
            };

            void insert(size_t id, T what)
            {

                size_t page = id / PAGE_SIZE;
                size_t index = id % PAGE_SIZE;
                if (page >= pages.size())
                {
                    resize(page * 2);
#if _SNEK_DEBUG_
                    LOG("basic storage T : " + snek::utils::type_descriptor<T>().stringify());
#endif
                }
                pages[page][index] = what;
            }

            [[nodiscard]] T get(size_t id) const noexcept
            {
                size_t page = id / PAGE_SIZE;
                size_t index = id % PAGE_SIZE;

                if (page < pages.size() && pages[page][index] != tombstone_v)
                {
                    return pages[page][index];
                }

                return tombstone_v;
            }

            [[nodiscard]] bool contains(size_t id) const noexcept
            {
                size_t page = id / PAGE_SIZE;
                size_t index = id % PAGE_SIZE;
                if (page < pages.size() && pages[page][index] != tombstone_v)
                {
                    return true;
                }
                return false;
            }

            ~page_storage() {};
        };

        // allows for multiple sparse sets of different component types to be pooled together
        struct polymorphic_sparse_set
        {
            virtual size_t size() const noexcept = 0;
            virtual bool contains(size_t id) const noexcept = 0;
            virtual void clear() = 0;
            virtual void remove(size_t id) = 0;
            virtual ~polymorphic_sparse_set() {};
        };

#include "../ecs/traits.hpp"

        template <typename T>
        class sparse_set : public polymorphic_sparse_set
        {
            // where T is a single component type
            std::vector<T> _packed_elements; // elements (components) in domain
            std::vector<size_t> _dense;      // 1 : 1 mapping of _packed_elements and index into _sparse
            std::vector<size_t> _sparse;     // will map the entity id to id in dense domain which is a 1:1 mapping to _packed_elements.

            constexpr static auto tombstone_v = snek::traits::tombstone_t<size_t>::null_v;

        public:
            sparse_set()
            {
                _sparse.resize(1000, tombstone_v);
            };
            sparse_set(size_t initial)
            {
                _sparse.resize(initial, tombstone_v);
            };

            void insert(size_t id, T elem)
            {
                _sparse[id] = _dense.size();
                _dense.push_back(id);
                _packed_elements.push_back(elem);
            }

            [[nodiscard]] size_t size() const noexcept override
            {
                return _dense.size();
            }
            // check if _dense has elem (id)
            [[nodiscard]] bool contains(size_t elem) const noexcept override
            {
                return (elem < _sparse.size() && _sparse[elem] < _dense.size() && _dense[_sparse[elem]] == elem);
            };

            void remove(size_t elem) override
            {
                const auto last = _dense.back();
                std::swap(_dense.back(), _dense[_sparse[elem]]);
                std::swap(_sparse[last], _sparse[elem]);
                _dense.pop_back();
            };
            void clear() override {};

            T *get(size_t id)
            {
            }

            ~sparse_set() {};
        };
    }

}
