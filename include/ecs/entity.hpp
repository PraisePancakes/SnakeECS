#pragma once
#include <iostream>
#include <cstdint>
#include "../common_types.hpp"
#include <vector>
#include "identifier.hpp"
#include <unordered_map>

namespace snek
{
    namespace core
    {
        class Component;
        class Entity
        {
            using df_uuid_t = u64;
            std::string tag;
            u64 cmp_mask; // bit set of components given its id;
            std::unordered_map<size_t, Component *> components;

        public:
            Entity() {};

            Entity(const Entity &other) {};
            Entity(Entity &&other) {};
            Entity(const std::string &tag) {};
            inline df_uuid_t GetID() const
            {
                return GenerateEntityID<df_uuid_t>();
            };

            template <typename C>
            bool HasComponent()
            {
                static_assert(std::is_base_of_v<Component, C>, "Custom component must be inherit from snek::core::Component");
                u64 hash = (GenerateComponentHashCode<C>());
                return ((cmp_mask & hash) == hash);
            };

            template <typename C, typename... Args>
            C &AddComponent(Args &&...args)
            {
                static_assert(std::is_base_of_v<Component, C>, "Custom component must be inherit from snek::core::Component");
                u64 hash = (GenerateComponentHashCode<C>());
                if (HasComponent<C>())
                {
                    return *((C *)components[hash]);
                }
                C *c = new C(std::forward<Args>(args)...);
                components[hash] = static_cast<C *>(c);
                cmp_mask |= hash;
                return *c;
            };

            template <typename C>
            void RemoveComponent()
            {
                static_assert(std::is_base_of_v<Component, C>, "Custom component must be inherit from snek::core::Component");
                cmp_mask &= ~(GenerateComponentHashCode<C>());
            };
            ~Entity() {};
        };

        // eventually the goal is to allow client side users of this library to use customizable ecs types
        //  so long as they inherit these types from the basis of this library : Entity , Component , Systems
    }
}