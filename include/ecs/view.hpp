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
        /*
            view.for_each([&](const T& t, const A& a) {
                std::cout << t.some_value();
                std::cout << a.some_value();
            });
        */

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
        light_view(ComponentStateTable st, GroupTable g) : _state_table(st), _groups(g) {};

        void for_each(std::function<void(Components &...)> f)
        {
            u64 group_mask = (uuid::GenerateComponentID<Components>() | ...);
            if (_groups.find(group_mask) == _groups.end())
                return;

            auto &entities = _groups[group_mask];
            auto &components = _state_table[group_mask];

            for (size_t i = 0; i < entities.size(); ++i)
            {
                if (entities[i] == nullptr) // Skip if the entity is null
                    continue;

                auto get_component = [&](auto *ptr) -> auto &
                {
                    using ComponentType = std::remove_pointer_t<decltype(ptr)>;
                    return *static_cast<ComponentType *>(ptr);
                };

                f(get_component(static_cast<Components *>(components[i]))...);
            }
        };

        ~light_view() {};
    };

}