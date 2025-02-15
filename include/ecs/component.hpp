#pragma once
#include <iostream>
#include <cstdint>
#include "../common_types.hpp"
#include <vector>
#include "identifier.hpp"
#include "entity.hpp"
#include "interface.hpp"

namespace snek
{
    namespace core
    {
        template <typename C>
        class Component : public BaseComponent
        {
            using df_size_t = u64;
            BaseEntity *owner;

        public:
            Component() {};
            df_size_t GetComponentHashCode() const
            {
                return GenerateComponentHashCode<C>();
            };

            ~Component() {};
        };

        class A : public Component<A>
        {
        };
        // eventually the goal is to allow client side users of this library to use customizable ecs types
        //  so long as they inherit these types from the basis of this library : Entity , Component , Systems
    }
}