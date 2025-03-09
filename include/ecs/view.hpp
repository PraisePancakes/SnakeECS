#pragma once
#include <tuple>
#include "../utils/identifier.hpp"
#include <functional>
#include <type_traits>
#include <array>
namespace snek
{

    template <u64 S, typename Pointer, typename... Components>
    class light_view
    {

        //  auto view = world.view<T, A>();

        using EntityArray = std::array<Pointer, S>;
        using ComponentArray = std::array<void *, S>;
        // where u64 is component mask over set of components.
        using GroupTable = std::unordered_map<u64, EntityArray>;
        using ComponentStateTable = std::unordered_map<u64, ComponentArray>;

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
            u64 cmp_mask = (uuid::GenerateComponentID<Components>() | ...);
            auto archetypes = _groups[cmp_mask];
            for (Pointer e : archetypes)
            {
                if (!e)
                    continue;
                f(*reinterpret_cast<Components *>(_state_table[uuid::GenerateComponentID<Components>()][e->GetID()])...);
            }
        };

        ~light_view() {};
    };

}