#pragma once
#include <memory>

namespace snek
{
    namespace internal
    {
        // TO DO : align allocated region such that padding gets fulfilled
        template <typename T, typename Alloc = std::allocator<T>>
        class snek_linear_allocator
        {
        public:
            typedef typename std::allocator_traits<Alloc> alloc_traits;
            typedef typename alloc_traits::value_type value_type;

        private:
            T *_region;
            Alloc _alloc;
            size_t _offset;
            size_t _max_size;
            // ---> memory linearity, linear allocator constructs entities linearly likewise destroys in a reverse linear fashion.
            // offset = 0
            // [  x  |   |   |   |   |   ]  construct at offset , offset++ => 1
            // [  x  | x |   |   |   |   ]  construct at offset , offset++ => 2
            // [  x  | x | x |   |   |   ]  construct at offset , offset++ => 3
            // [  x  | x |   |   |   |   ]  destroy at offset   , offset-- => 4
            // [  x  |   |   |   |   |   ]  destroy at offset   , offset-- => 5
            // the memory is already preallocated all we need to do is construct at these offsets saving from potential mass "new" bottlenecks.

            [[nodiscard]] T *top() const
            {
                return _region + (_offset - 1);
            };

            void pop()
            {
                destroy(std::move(top()));
                _offset--;
            }

        public:
            snek_linear_allocator() : _region(), _max_size(), _offset(0) {};
            snek_linear_allocator(const snek_linear_allocator &other) : _region(other._region), _max_size(other._max_size), _offset(other._offset) {};
            snek_linear_allocator(snek_linear_allocator &&other) : _region(std::move(other._region)), _max_size(other._max_size), _offset(other._offset) {};

            T *allocate(const size_t n)
            {
                _max_size = n;
                try
                {
                    _region = std::allocator_traits<Alloc>::allocate(_alloc, n);
                    return _region;
                }
                catch (std::exception &e)
                {
                    std::cerr << e.what() << std::endl;
                    return nullptr;
                };
            };

            template <typename... Args>
            T *construct(Args &&...args)
            {
                push(std::move(args)...);
                return top();
            };

            template <typename U>
            constexpr static void destroy(U *p)
            {
                std::destroy_at(p);
            };

            template <typename... Args>
            void push(Args &&...args)
            {
                _offset++;
                if (_offset > _max_size)
                {
                    _offset--;
                    return;
                }
                std::construct_at(_region + (_offset - 1), std::forward<Args>(args)...);
            }

            void deallocate() {

            };

            template <typename U>
            struct rebind
            {
                typedef snek_linear_allocator<T> other;
            };

            ~snek_linear_allocator() {

            };
        };
    }
}