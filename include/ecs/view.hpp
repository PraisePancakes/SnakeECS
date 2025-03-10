#pragma once
#include <tuple>
#include "../utils/identifier.hpp"
#include <functional>
#include <type_traits>
#include <array>
namespace snek
{



    template <u64 S, typename WorldType, typename Pointer, typename... Components>
    class light_view
    {
        using underlying_type = WorldType;
        //  auto view = world.view<T, A>();
        using EntityArray = WorldType::EntityArray;
        using ComponentArray = WorldType::ComponentArray;
        // where u64 is component mask over set of components.
        using GroupTable = WorldType::GroupTable;
        using ComponentStateTable = WorldType::ComponentStateTable;

        using alloc_traits = WorldType::alloc_traits;
        using value_type = WorldType::value_type;
        using reference = WorldType::reference;
        using const_reference = WorldType::const_reference;
        using pointer = WorldType::pointer;
        using EntityID = WorldType::EntityID;
        using ComponentID = WorldType::ComponentID;
        using ComponentMask = WorldType::ComponentMask;

        static_assert((std::is_class_v<Components> && ...),
                      "Component must pass class type trait");

        ComponentStateTable _state_table;
        GroupTable _groups;

    public:
        light_view(ComponentStateTable &st, GroupTable &g) : _state_table(st), _groups(g) {};

        /*
            view.for_each([&](const T& t, const A& a) {
                std::cout << t.some_value();
                std::cout << a.some_value();
            });
        */
        void for_each(std::function<void(Components &...)> f)
        {
            ComponentMask cmp_mask = (uuid::GenerateComponentID<Components>() | ...);
            auto archetypes = _groups[cmp_mask];
            for (const auto &e : archetypes)
            {
                if (!e)
                    continue;
                f(*reinterpret_cast<Components *>(_state_table[uuid::GenerateComponentID<Components>()][e->GetID()])...);
            }
        };

        ~light_view() {};
    };

}