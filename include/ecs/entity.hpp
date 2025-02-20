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
    public:
        using mask_t = u64;
        using tag_t = std::string;
        using size_type = u64;

    protected:
        tag_t tag;
        mask_t cmp_mask; // bit mask of components
        std::unordered_map<size_t, Component *> components;
        bool is_alive;

    public:
        Entity() : tag(), cmp_mask(), components(), is_alive(true) {};
        Entity(const Entity &other) : tag(other.tag), cmp_mask(), components(other.components), is_alive(other.is_alive) {};
        Entity(Entity &&other) : tag(other.tag), cmp_mask(), components(other.components), is_alive(other.is_alive) {};
        Entity(const std::string &tag) : tag(tag), cmp_mask(), components(), is_alive(true) {};

        [[nodiscard]] inline size_type GetID() const noexcept
        {
            return uuid::GenerateEntityID<size_type>();
        };

        [[nodiscard]] inline size_type GetComponentMask() const noexcept
        {
            return cmp_mask;
        }

        [[nodiscard]] inline const tag_t &GetTag() const noexcept
        {
            return tag;
        }

        template <typename C>
        [[nodiscard]] bool HasComponent() noexcept
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
            c->owner = this;
            return *c;
        };

        [[nodiscard]] bool IsAlive() const
        {
            return this->is_alive;
        }

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