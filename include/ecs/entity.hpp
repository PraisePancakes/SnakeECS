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
        bool is_alive;                                              
        size_t _id;                                                 

    public:
        Entity() : _id(), cmp_mask(), is_alive(true) {};
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

        void SetID(u64 id) noexcept
        {
            _id = id;
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