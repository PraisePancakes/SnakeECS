#pragma once
#include <tuple>

namespace snek
{
    template <typename... Components>
    class light_view
    {
        // used as so ...
        //  auto view = world.view<T, A>();
        /*
            view.for_each([&](const T& t, const A& a) {
                std::cout << t.some_value();
                std::cout << a.some_value();
            });
        */
    public:
        using Types = std::tuple<Components...>;
        Types types;

        light_view() {};
        ~light_view() {};
    };
}