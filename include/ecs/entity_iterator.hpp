#pragma once
#include <iostream>

namespace snek
{
    namespace container
    {
        template <typename T>
        class entt_iterator
        {
            T *it;

        public:
            using value_t = T;
            using pointer = T *;
            using size_type = size_t;
            using mask_t = T::mask_t;
            using difference_type = std::ptrdiff_t;
            using reference = T &;
            using const_reference = const T &;
            using const_pointer = const T *;

            entt_iterator() : it(nullptr) {};
            entt_iterator(const entt_iterator &o) : it(o.it) {};
            entt_iterator(entt_iterator &&o) : it(std::move(o.it)) {};
            entt_iterator(T *ptr) : it(ptr) {};

            entt_iterator &operator++()
            {
                it++;
                return *this;
            };

            entt_iterator &operator++(difference_type)
            {
                entt_iterator ret = *this;
                ++(*this);
                return ret;
            }

            bool operator==(const entt_iterator &o)
            {
                return this->it == o.it;
            }

            bool operator!=(const entt_iterator &o)
            {
                return !(this == o);
            }

            ~entt_iterator() {};
        };

        template <typename It>
        struct entt_iterator_traits
        {
            using value_t = It::value_t;
            using size_type = It::size_type;
            using mask_t = It::mask_t;
            using tag_t = It::tag_t;
            using difference_type = It::difference_type;
            using pointer = It::pointer;
            using reference = It::reference;
        };
    }
}