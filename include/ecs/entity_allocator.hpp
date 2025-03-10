#pragma once
#include "entity.hpp"

namespace snek
{
    namespace allocator
    {
        template <typename T>
        class entity_allocator
        {
        public:
            using value_type = snek::entity_traits<T>::value_type;
            using pointer = value_type *;
            using reference = value_type &;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;
            using const_reference = const value_type &;
            using const_pointer = const value_type *;
            using void_pointer = void *;
            template <typename U>
            struct rebind
            {
                using other = entity_allocator<U>;
            };

        public:
            template <typename U>

            entity_allocator(const entity_allocator<U> &alloc){};
            constexpr pointer allocate(size_type n)
            {
                return static_cast<pointer>(::operator new(n * sizeof(value_type)));
            };

            constexpr void deallocate(pointer p, size_type n)
            {
                ::operator delete(p);
            };

            entity_allocator() {};
            ~entity_allocator() {};
        };

    }
}
