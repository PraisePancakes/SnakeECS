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
    World<10> w;

    auto e = w.Spawn();

    w.BindComponent<A>(e, 4);
    w.BindComponent<B>(e, 'x');

    auto g = w.GetGroupView<A, B>();


    return 0;
}