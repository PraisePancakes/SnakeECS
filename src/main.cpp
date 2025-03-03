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

    char c = 'a';
    B(char c) : c(c) {};
    ~B() {};
};

struct C : public Component
{
};

struct D : public Component
{
};

int main(int argc, char **argv)
{
    World<11> w;

    auto e1 = w.Spawn();
    w.BindComponent<A>(e1);

    for (int i = 0; i < 10; i++)
    {
        auto e = w.Spawn();
        w.InitializeComponents<A, B>(e, i, i + 97);
    }

    auto group = w.GetGroupView<A, B>();
    auto group2 = w.GetGroupView<A>();

    return 0;
}