#pragma once
#include <iostream>
#include <cstdint>
#include "../common_types.hpp"
#include <vector>
#include "../utils/identifier.hpp"
#include <unordered_map>

namespace snek
{

    class Component;
    class Entity
    {
        using df_uuid_t = u64;
        std::string tag;
        u64 cmp_mask; // bit mask of components
        std::unordered_map<size_t, Component *> components;

    public:
        Entity() : tag(), cmp_mask(), components() {};
        Entity(const Entity &other) : tag(other.tag), cmp_mask(), components(other.components) {};
        Entity(Entity &&other) : tag(other.tag), cmp_mask(), components(other.components) {};
        Entity(const std::string &tag) : tag(tag), cmp_mask(), components() {};

        inline df_uuid_t GetID() const
        {
            return uuid::GenerateEntityID<df_uuid_t>();
        };

        template <typename C>
        bool HasComponent()
        {
            static_assert(std::is_base_of_v<Component, C>, "Custom component must be inherit from snek::core::Component");
            u64 hash = (uuid::GenerateComponentHashCode<C>());
            return ((cmp_mask & hash) == hash);
        };

        template <typename C, typename... Args>
        C &AddComponent(Args &&...args)
        {
            static_assert(std::is_base_of_v<Component, C>, "Custom component must be inherit from snek::core::Component");
            u64 hash = (uuid::GenerateComponentHashCode<C>());
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
            cmp_mask &= ~(uuid::GenerateComponentHashCode<C>());
        };
        ~Entity() {};
    };

    // eventually the goal is to allow client side users of this library to use customizable ecs types
    //  so long as they inherit these types from the basis of this library : Entity , Component , Systems

}