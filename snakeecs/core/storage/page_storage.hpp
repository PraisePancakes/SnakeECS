#pragma once
#include "../../debug/debug.hpp"
#include "../../utils/type_descriptor.hpp"
#include "../../ecs/traits.hpp"
#include <iostream>
#include <vector>
#include <memory>

namespace snek
{
    namespace storage
    {
#define PAGE_SIZE 4
        template <typename T, typename Alloc>
        class page_storage
        {
            typedef std::array<T, PAGE_SIZE> page_t;
            typedef std::allocator_traits<Alloc>::template rebind_alloc<page_t> internal_allocator;
            std::vector<page_t, internal_allocator> pages;
            size_t _size = 0;

            static constexpr auto tombstone_v = snek::traits::tombstone_t<T>::null_v;
            void resize(size_t new_size)
            {
                std::array<T, PAGE_SIZE> arr;
                arr.fill(tombstone_v);
                pages.resize(new_size, arr);
            }

        public:
            template <typename U>
            class _flat_page_iterator
            {
                std::vector<std::array<U, PAGE_SIZE>, internal_allocator> &cont;

                size_t outer_index = 0;
                size_t inner_index = 0;

            public:
                using value_type = U;
                using pointer = U *;
                using const_pointer = const U *;
                using reference = U &;
                using const_reference = const U &;
                using size_type = size_t;
                using difference_type = std::ptrdiff_t;
                using iterator_category = std::bidirectional_iterator_tag;

                _flat_page_iterator(std::vector<std::array<U, PAGE_SIZE>, internal_allocator> &container, size_t out, size_t in)
                    : cont(container), outer_index(out), inner_index(in) {

                      };

                _flat_page_iterator(const _flat_page_iterator &o)
                    : cont(o.cont), outer_index(o.outer_index), inner_index(o.inner_index) {};

                _flat_page_iterator(_flat_page_iterator &&o)
                    : cont(std::move(o.cont)), outer_index(o.outer_index), inner_index(o.inner_index) {};

                bool operator==(const _flat_page_iterator &other)
                {
                    return this->cont == other.cont && this->outer_index == other.outer_index && this->inner_index == other.inner_index;
                };

                bool operator!=(const _flat_page_iterator &other)
                {
                    return !(*this == other);
                };

                _flat_page_iterator &operator++()
                {
                    if (inner_index < PAGE_SIZE - 1)
                    {
                        inner_index++;
                    }
                    else
                    {
                        if (outer_index < cont.size())
                        {
                            outer_index++;
                            inner_index = 0;
                        }
                    }
                    return *this;
                }

                _flat_page_iterator &operator--()
                {
                    if (inner_index > 0)
                    {
                        inner_index--;
                    }
                    else
                    {
                        if (outer_index > 0)
                        {
                            outer_index--;
                            inner_index = PAGE_SIZE - 1;
                        }
                    }
                    return *this;
                }

                U &operator->()
                {
                    return *this;
                };

                const U &operator->() const
                {
                    return *this;
                };

                U &operator*()
                {
                    return cont[outer_index][inner_index];
                };

                const U &operator*() const
                {
                    return *this;
                };

                ~_flat_page_iterator() {};
            };

            template <typename U>
            class _flat_page_iterator<const U>
            {
                std::vector<std::array<U, PAGE_SIZE>, internal_allocator> &cont;

                size_t outer_index = 0;
                size_t inner_index = 0;

            public:
                using value_type = U;
                using pointer = U *;
                using const_pointer = const U *;
                using reference = U &;
                using const_reference = const U &;
                using size_type = size_t;
                using difference_type = std::ptrdiff_t;
                using iterator_category = std::bidirectional_iterator_tag;

                _flat_page_iterator(std::vector<std::array<U, PAGE_SIZE>, internal_allocator> &container, size_t out, size_t in)
                    : cont(container), outer_index(out), inner_index(in) {

                      };
                _flat_page_iterator(const _flat_page_iterator &o)
                    : cont(o.cont), outer_index(o.outer_index), inner_index(o.inner_index) {};

                _flat_page_iterator(_flat_page_iterator &&o)
                    : cont(std::move(o.cont)), outer_index(o.outer_index), inner_index(o.inner_index) {};

                bool operator==(const _flat_page_iterator &other)
                {
                    return this->cont == other.cont && this->outer_index == other.outer_index && this->inner_index == other.inner_index;
                };

                bool operator!=(const _flat_page_iterator &other)
                {
                    return !(*this == other);
                };
                _flat_page_iterator &operator++()
                {
                    if (inner_index < PAGE_SIZE - 1)
                    {
                        inner_index++;
                    }
                    else
                    {
                        if (outer_index < cont.size())
                        {
                            outer_index++;
                            inner_index = 0;
                        }
                    }
                    return *this;
                }
                _flat_page_iterator &operator--()
                {
                    if (inner_index > 0)
                    {
                        inner_index--;
                    }
                    else
                    {
                        if (outer_index > 0)
                        {
                            outer_index--;
                            inner_index = PAGE_SIZE - 1;
                        }
                    }
                    return *this;
                }
                const U &operator->() const
                {
                    return *this;
                };

                const U &operator*() const
                {
                    return cont[outer_index][inner_index];
                };

                ~_flat_page_iterator() {};
            };

            using iterator = _flat_page_iterator<T>;
            using const_iterator = _flat_page_iterator<const T>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            iterator begin()
            {
                return iterator(pages, 0, 0);
            }

            iterator end()
            {
                return iterator(pages, _size / PAGE_SIZE, (_size) % PAGE_SIZE);
            }

            reverse_iterator rbegin()
            {
                auto it = iterator(pages, _size / PAGE_SIZE, (_size) % PAGE_SIZE);
                return reverse_iterator(it);
            }

            reverse_iterator rend()
            {
                auto it = iterator(pages, 0, 0);
                return reverse_iterator(it);
            }

            const_iterator cbegin()
            {
                return const_iterator(pages, 0, 0);
            }

            const_iterator cend()
            {
                return const_iterator(pages, _size / PAGE_SIZE, (_size) % PAGE_SIZE);
            }

            const_reverse_iterator crbegin()
            {
                const auto it = const_iterator(pages, _size / PAGE_SIZE, (_size) % PAGE_SIZE);
                return const_reverse_iterator(it);
            }

            const_reverse_iterator crend()
            {
                const auto it = const_iterator(pages, 0, 0);
                return const_reverse_iterator(it);
            }

            page_storage()
            {
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
                    resize((page + 1) * 2);
#if _SNEK_DEBUG_
                    LOG("basic storage T : " + snek::utils::type_descriptor<T>().stringify());
#endif
                }
                if (!(_size >= tombstone_v))
                {
                    _size++;
                }
                pages[page][index] = what;
            }

            void insert(size_t id)
            {

                size_t page = id / PAGE_SIZE;
                size_t index = id % PAGE_SIZE;
                if (page >= pages.size())
                {
                    resize((page + 1) * 2);
#if _SNEK_DEBUG_
                    LOG("basic storage T : " + snek::utils::type_descriptor<T>().stringify());
#endif
                }
                ++_size;
                pages[page][index] = id;
            }

            [[nodiscard]] T get(size_t id) const noexcept
            {
                size_t page = id / PAGE_SIZE;
                size_t index = id % PAGE_SIZE;

                if (page < pages.size())
                {
                    return pages[page][index];
                }

                return tombstone_v;
            }

            void pop() noexcept
            {
                if (!(_size >= 0))
                    return;

                size_t npages = this->pages.size();
                size_t index = PAGE_SIZE;
                pages[npages][index] = tombstone_v;
                _size--;
            };

            [[nodiscard]] bool contains(size_t id) const noexcept
            {
                size_t page = id / PAGE_SIZE;
                size_t index = id % PAGE_SIZE;
                if (page < pages.size())
                {
                    return true;
                }
                return false;
            }

            size_t size() const noexcept
            {
                return _size;
            };

            ~page_storage() {};
        };

    }
}