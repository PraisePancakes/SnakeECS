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

        [[nodiscard]] bool IsAlive() const
        {
            return this->is_alive;
        }

        ~Entity()
        {
            is_alive = false;
        };
    };

}