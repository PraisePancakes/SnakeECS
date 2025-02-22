#pragma once
#include <iostream>

namespace snek
{
    namespace container
    {
        template <typename T>
        class entt_iterator : public std::iterator<std::input_iterator_tag, T, std::ptrdiff_t, T *, T &>
        {
            T *it;

        public:
            using mask_t = T::mask_t;
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

            bool operator==(const entt_iterator &o) const
            {
                return this->it == o.it;
            }

            bool operator!=(const entt_iterator &o) const
            {
                return !(*this == o);
            }

            reference operator*() const
            {
                return *it;
            };

            ~entt_iterator() {};
        };

        template <typename It>
        struct entt_iterator_traits : public std::iterator_traits<It>
        {
            using mask_t = It::mask_t;
            using tag_t = It::tag_t;
        };
    }
}