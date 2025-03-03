#pragma once
#include <iostream>
#include "../common_types.hpp"

namespace snek
{
    namespace container
    {

        template <typename T, u64 n>
        class sparse_set
        {
        public:
            static constexpr max_size = n;

        private:
            std::vector<std::array<T, max_size>> _pages;
            std::vector<T> _dense;
            std::vector<u64> _ids;

        public:
            sparse_set() {};
            ~sparse_set() {};
        };
    }
}