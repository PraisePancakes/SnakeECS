#pragma once

namespace snek::core
{

    template <typename T>
    class verbose_allocator
    {
    public:
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using void_pointer = void *;
        using const_void_pointer = const void *;
        using const_pointer = const T *;
        using const_reference = const T &;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;

        verbose_allocator() {};
        template <typename U>
        struct rebind
        {
            using other = verbose_allocator<U>;
        };

        template <typename U>
        verbose_allocator(const verbose_allocator<U> &alloc){};

        constexpr pointer allocate(size_type n)
        {
            std::cout << "[ ALLOCATED " << n * sizeof(value_type) << " bytes ] where sizeof T : " << sizeof(value_type) << std::endl;
            return static_cast<pointer>(::operator new(n * sizeof(value_type)));
        };

        constexpr void deallocate(pointer p, size_type n)
        {
            std::cout << "[ DEALLOCATED " << n * sizeof(value_type) << " ] bytes where sizeof T : " << sizeof(value_type) << std::endl;
            ::operator delete(p);
        };

        ~verbose_allocator() = default;
    };
};
