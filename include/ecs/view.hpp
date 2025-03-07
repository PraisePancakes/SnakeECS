#pragma once
#include <tuple>
#include "../utils/identifier.hpp"
#include <functional>
#include <type_traits>
#include <array>
namespace snek
{

    template <u64 S, typename... Components>
    class light_view
    {

        //  auto view = world.view<T, A>();
        /*
            view.for_each([&](const T& t, const A& a) {
                std::cout << t.some_value();
                std::cout << a.some_value();
            });
        */

        using ComponentArray = std::array<void *, S>;
        using ComponentStateTable = std::unordered_map<u64, ComponentArray>;
        static constexpr u64 group_mask = (uuid::GenerateComponentID<Components>() | ...);

        static_assert((std::is_class_v<Components> && ...),
                      "Component must pass class type trait");

        ComponentStateTable _state_table;

    public:
        using Types = std::tuple<Components...>;
        Types types;

        light_view(ComponentStateTable st) : _state_table(st) {};

        template <typename Func>
        void for_each(std::function<Func> f) {
            
        };

        ~light_view() {};
    };

}