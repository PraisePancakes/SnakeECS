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

    protected:
        tag_t tag;
        mask_t cmp_mask; // bit mask of components
        std::unordered_map<size_t, Component *> components;
        bool is_alive;
        size_t _id;

    public:
        Entity() : _id(uuid::GenerateEntityID<u64>()), tag(), cmp_mask(), components(), is_alive(true) {};
        Entity(const Entity &other) : _id(other._id), tag(other.tag), cmp_mask(), components(other.components), is_alive(other.is_alive) {};
        Entity(Entity &&other) : _id(other._id), tag(other.tag), cmp_mask(), components(other.components), is_alive(other.is_alive) {};
        Entity(const std::string &tag) : _id(uuid::GenerateEntityID<u64>()), tag(tag), cmp_mask(), components(), is_alive(true) {};

        [[nodiscard]] inline u64 GetID() const noexcept
        {
            return _id;
        };

        [[nodiscard]] inline mask_t GetComponentMask() const noexcept
        {
            return cmp_mask;
        }

        [[nodiscard]] inline const tag_t &GetTag() const noexcept
        {
            return tag;
        }

        template <typename T>
        [[nodiscard]] bool HasComponent() const noexcept
        {
            static_assert(std::is_base_of_v<Component, T>, "Custom component must inherit from snek::core::Component");
            u64 hash = (uuid::GenerateComponentHashCode<T>());
            return ((cmp_mask & hash) == hash);
        };

        template <typename T, typename U, typename... Args>
        [[nodiscard]] bool HasComponent() const noexcept
        {
            return (HasComponent<T>() && HasComponent<U>() && (HasComponent<Args>() && ...));
        };

        template <typename C, typename... Args>
        C &AddComponent(Args &&...args)
        {
            static_assert(std::is_base_of_v<Component, C>, "Custom component must inherit from snek::core::Component");
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

        template <typename... T, typename... Args>
        void InitializeComponents(Args &&...args)
        {
            (AddComponent<T>(std::forward<Args>(args)), ...);
        }

        template <typename T>
        T *GetComponent() const
        {
            static_assert(std::is_base_of_v<Component, T>, "Custom component must inherit from snek::core::Component");
            if (!HasComponent<T>())
                return nullptr;
            u64 hash = (uuid::GenerateComponentHashCode<T>());
            return (T *)components.at(hash);
        };

        [[nodiscard]] bool IsAlive() const
        {
            return this->is_alive;
        }

        void kill()
        {
            this->is_alive = false;
        }

        template <typename T>
        void RemoveComponent() noexcept
        {
            if (!HasComponent<T>())
                return;
            static_assert(std::is_base_of_v<Component, T>, "Custom component must inherit from snek::core::Component");
            u64 hash = (uuid::GenerateComponentHashCode<T>());
            cmp_mask &= ~(hash);
        };

        template <typename T, typename U, typename... Args>
        void RemoveComponent() noexcept
        {
            RemoveComponent<T>();
            RemoveComponent<U>();
            (RemoveComponent<Args>(), ...);
        };

        ~Entity() {};
    };

    // eventually the goal is to allow client side users of this library to use customizable ecs types
    //  so long as they inherit these types from the basis of this library : Entity , Component , Systems

}