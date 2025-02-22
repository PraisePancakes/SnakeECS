#pragma once
#include <memory>

namespace snek
{
    namespace internal
    {
        template <typename S, typename T, typename Alloc = std::allocator<T>>
        class snek_allocator
        {
            T *_region;
            Alloc _alloc;

        public:
            snek_allocator() : _region(_alloc.allocate(100)) {};
            snek_allocator(S size) : _region(_alloc.allocate(size)) {};
            snek_allocator(const snek_allocator &other) : _region(other._region) {};
            snek_allocator(snek_allocator &&other) : _region(std::move(other._region)) {};
            T *request(S size) {
                // request a region of size from pool;
            };
            std::allocator<T> &get_allocator() const { return _alloc; };
            ~snek_allocator() {

            };
        };
    }
}