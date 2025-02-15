#pragma once
#include <iostream>
#include <cstdint>
#include "../common_types.hpp"
#include <vector>
#include "identifier.hpp"
#include "interface.hpp"
#include <unordered_map>

namespace snek
{
    namespace core
    {

        class Entity : public BaseEntity
        {
            using df_size_t = u64;
            std::string tag;
            u64 cmp_mask; // bit set of components given its id;
            std::unordered_map<size_t, BaseComponent *> components;

        public:
            Entity() {};

            Entity(const Entity &other) {};
            Entity(Entity &&other) {};
            Entity(const std::string &tag) {};
            inline df_size_t GetID() const
            {
                return GenerateEntityID<df_size_t>();
            };

            template <typename Component>
            bool HasComponent()
            {
                u64 mask = (GenerateComponentHashCode<Component>());
                return ((cmp_mask & mask) == mask);
            };

            template <typename Component, typename... Args, typename = std::enable_if_t<std::is_base_of_v<BaseComponent, Component>>>
            Component &AddComponent(Args &&...args)
            {

                u64 mask = (GenerateComponentHashCode<Component>());
                if (HasComponent<Component>())
                {
                    return *((Component *)components[mask]);
                }
                Component *c = new Component(std::forward<Args>(args)...);
                components[mask] = static_cast<Component *>(c);
                cmp_mask |= mask;
                return *c;
            };

            template <typename Component, typename = std::enable_if_t<std::is_base_of_v<BaseComponent, Component>>>
            void RemoveComponent()
            {
                cmp_mask &= ~(GenerateComponentHashCode<Component>());
            };
            ~Entity() {};
        };

        // eventually the goal is to allow client side users of this library to use customizable ecs types
        //  so long as they inherit these types from the basis of this library : Entity , Component , Systems
    }
}