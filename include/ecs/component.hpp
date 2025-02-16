#pragma once
#include <iostream>
#include <cstdint>
#include "../common_types.hpp"
#include <vector>
#include "identifier.hpp"
#include "entity.hpp"

namespace snek
{
    namespace core
    {
        class Component
        {
            using df_hash_t = u64;
            Entity *owner;

        public:
            Component() {};
            ~Component() {};
        };

        class A : public Component
        {
        };
        // eventually the goal is to allow client side users of this library to use customizable ecs types
        //  so long as they inherit these types from the basis of this library : Entity , Component , Systems
    }
}