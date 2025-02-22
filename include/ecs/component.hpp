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
    public:
        Entity *owner;
        Component() {};
        ~Component() {};
    };

    // eventually the goal is to allow client side users of this library to use customizable ecs types
    //  so long as they inherit these types from the basis of this library : Entity , Component , Systems

}