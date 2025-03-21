#pragma once
#include "../debug/debug.hpp"
#include <iostream>
#include <vector>
#include "../utils/type_descriptor.hpp"
#include "../ecs/traits.hpp"

namespace snek
{
    namespace storage
    {
#define PAGE_SIZE 4
        template <typename T>
        class page_storage
        {

            std::vector<std::array<T, PAGE_SIZE>> pages;
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

            [[nodiscard]] T get(size_t id)
            {
                size_t page = id / PAGE_SIZE;
                size_t index = id % PAGE_SIZE;

                if (page < pages.size() && pages[page][index] != tombstone_v)
                {
                    return pages[page][index];
                }

                return tombstone_v;
            }

            ~page_storage() {};
        };
    }

}
