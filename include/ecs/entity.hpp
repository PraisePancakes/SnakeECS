#pragma once
#include <iostream>
#include "../common_types.hpp"
#include "../utils/identifier.hpp"

namespace snek
{
    class Entity
    {
    protected:
        u64 cmp_mask; // bit mask of components
        // std::unordered_map<size_t, Component *> components;
        bool is_alive;
        size_t _id;

    public:
        Entity() : _id(uuid::GenerateEntityID()), cmp_mask(), is_alive(true) {};
        Entity(const Entity &other) : _id(other._id), cmp_mask(), is_alive(other.is_alive) {};
        Entity(Entity &&other) : _id(other._id), cmp_mask(), is_alive(other.is_alive) {};

        [[nodiscard]] inline u64 GetID() const noexcept
        {
            return _id;
        };

        [[nodiscard]] inline u64 GetComponentMask() const noexcept
        {
            return cmp_mask;
        }

        void SetComponentFlag(u64 f)
        {
            cmp_mask |= f;
        };

        void RemoveComponentFlag(u64 f)
        {
            cmp_mask &= ~(f);
        }

        // template <typename T>
        // [[nodiscard]] bool HasComponent() const noexcept
        // {
        //     static_assert(std::is_base_of_v<Component, T>, "Custom component must inherit from snek::core::Component");
        //     Component *c = (Component *)T();
        //     u64 hash = (c->Id);
        //     return ((cmp_mask & hash) == hash);
        // };

        // template <typename T, typename U, typename... Args>
        // [[nodiscard]] bool HasComponent() const noexcept
        // {
        //     return (HasComponent<T>() && HasComponent<U>() && (HasComponent<Args>() && ...));
        // };

        // template <typename C, typename... Args>
        // C &AddComponent(Args &&...args)
        // {
        //     static_assert(std::is_base_of_v<Component, C>, "Custom component must inherit from snek::core::Component");
        //     u64 hash = (uuid::GenerateComponentHashCode<C>());
        //     if (HasComponent<C>())
        //     {
        //         return *((C *)components[hash]);
        //     }
        //     C *c = new C(std::forward<Args>(args)...);
        //     components[hash] = static_cast<C *>(c);
        //     cmp_mask |= hash;
        //     c->owner = this;
        //     return *c;
        // };

        // template <typename... T, typename... Args>
        // void InitializeComponents(Args &&...args)
        // {
        //     (AddComponent<T>(std::forward<Args>(args)), ...);
        // }

        // template <typename T>
        // T *GetComponent() const
        // {
        //     static_assert(std::is_base_of_v<Component, T>, "Custom component must inherit from snek::core::Component");
        //     if (!HasComponent<T>())
        //         return nullptr;
        //     u64 hash = (uuid::GenerateComponentHashCode<T>());
        //     return (T *)components.at(hash);
        // };

        // template <typename T>
        // void RemoveComponent() noexcept
        // {
        //     if (!HasComponent<T>())
        //         return;
        //     static_assert(std::is_base_of_v<Component, T>, "Custom component must inherit from snek::core::Component");
        //     u64 hash = (uuid::GenerateComponentHashCode<T>());
        //     cmp_mask &= ~(hash);
        // };

        // template <typename T, typename U, typename... Args>
        // void RemoveComponent() noexcept
        // {
        //     RemoveComponent<T>();
        //     RemoveComponent<U>();
        //     (RemoveComponent<Args>(), ...);
        // };

        // [[nodiscard]] bool IsAlive() const
        // {
        //     return this->is_alive;
        // }

        // void Kill()
        // {
        //     this->is_alive = false;
        //     std::for_each(components.begin(), components.end(), [](std::pair<size_t, Component *> pair)
        //                   { auto c = pair.second;

        //       if(c) {
        //         delete c; } });
        // }

        ~Entity() {

        };
    };

    // eventually the goal is to allow client side users of this library to use customizable ecs types
    //  so long as they inherit these types from the basis of this library : Entity , Component , Systems

}