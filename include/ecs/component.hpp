#pragma once
#include <iostream>
#include <cstdint>
#include "../common_types.hpp"
#include <vector>
#include "../utils/identifier.hpp"
#include "entity.hpp"

namespace snek
{
    class Entity;
    class Component
    {
        using hash_type = u64;

    public:
        Entity *owner;
        Component() {};
        ~Component() {};
    };

    class A : public Component
    {
        int x;
    };
    // eventually the goal is to allow client side users of this library to use customizable ecs types
    //  so long as they inherit these types from the basis of this library : Entity , Component , Systems

}