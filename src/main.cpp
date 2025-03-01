#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"

using namespace snek;
using namespace snek::internal;

struct A : public Component
{
    int x = 2;
    A() {};
    A(int x) : x(x) {};
};

struct B : public Component
{
};

int main(int argc, char **argv)
{
    World<1000> w;
    auto e = w.Spawn();
    auto c = w.BindComponent<A>(e);
    auto d = w.BindComponent<A>(e, 3);

    std::cout << w.HasComponent<B>(e);

    return 0;
}