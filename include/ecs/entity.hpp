#pragma once
#include <iostream>
#include <cstdint>
#include "../common_types.hpp"
#include <vector>

namespace snek
{
    namespace core
    {

        template <typename SizeT = u64>
        inline auto GenerateEntityID() noexcept
        {
            static SizeT new_id = 0;
            SizeT old = new_id;
            new_id++;
            return old;
        };

        template <typename SizeT = u64>
        struct EntityIdentifier
        {
            typedef SizeT size_type;
            size_type id;
            EntityIdentifier() : id(GenerateEntityID<size_type>()) {};
        };

        class Entity : public EntityIdentifier<u64>
        {
            using df_size_t = u64;
            std::string tag;
            std::uint64_t cmp_mask; // bit set of components given its id;

        public:
            Entity() {};
            Entity(const Entity &other) {};
            Entity(Entity &&other) {};
            Entity(const std::string &tag) {};
            inline df_size_t GetID() const
            {
                return this->id;
            };
            ~Entity() {};
        };

        template <typename Component>
        size_t GenerateComponentTypeID()
        {
            return typeid(Component).hash_code();
        };

        template <typename Component>
        struct ComponentIndentifier
        {
            typedef size_t size_type;
            size_type id;
            ComponentIndentifier() : id(GenerateComponentTypeID<Component>()) {};
        };

        template <typename Component>
        class BaseComponent : public ComponentIndentifier<Component>
        {
            using df_size_t = u64;
            Entity *owner;

        public:
            BaseComponent() {};
            df_size_t GetComponentTypeID() const
            {
                return this->id;
            };

            ~BaseComponent() {};
        };

        class A : public BaseComponent<A>
        {
        };

        // eventually the goal is to allow client side users of this library to use customizable ecs types
        //  so long as they inherit these types from the basis of this library : Entity , Component , Systems
    }
}