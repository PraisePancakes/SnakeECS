#include "../include/ecs/world.hpp"

static snek::world<1000> world;
void TEST_ENTITY_ID()
{
    for (size_t i = 0; i < 10; i++)
    {
        snek::entity e = world.spawn();
    }
};

struct A
{
    int x = 2;
    A() {};
    A(int x) : x(x) {};
};

struct B
{

    char c = 'a';
    B(char c) : c(c) {};
    ~B() {};
};

struct C
{
    int x = 2;
    C() {};
    C(int x) : x(x) {};
};

struct D
{
    char c = 'a';
    D(char c) : c(c) {};
    ~D() {};
};

void TEST_COMPONENTS()
{
    std::cout << "[[ COMPONENT TEST ]]\n.\n.\n." << std::endl;

    for (size_t i = 0; i < 10; i++)
    {
        snek::entity e = world.spawn();
        world.bind<A>(e, (i % 2 == 0 ? i + 1 : i - 1));
        std::cout << "Entity with id : " << i << " has A component [ " << world.get_component<A>(e)->x << " ]" << std::endl;
    }
};

void TEST_VIEW_SINGLE_COMPONENT_POLICY()
{
    std::cout << "[[ VIEW FOR EACH : STRICTLY ONE COMPONENT ]]\n.\n.\n." << std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        snek::entity e = world.spawn();
        world.initialize<A, B>(e, (i % 2 == 0 ? i + 1 : i - 1), i + 97);
    }
    auto view = world.view<A>();
    view.for_each([](A &a)
                  { std::cout << a.x << std::endl; });
};

void TEST_COMPONENT_LIST_INITIALIZER()
{
    std::cout << "[[ COMPONENT LIST INITIALIZER TEST ]]\n.\n.\n." << std::endl;

    for (size_t i = 0; i < 10; i++)
    {
        snek::entity e = world.spawn();
        world.initialize<A, B>(e, (i % 2 == 0 ? i + 1 : i - 1), i + 97);
        std::cout << "Entity with id : " << i << std::endl;
        std::cout << " has A component [ " << world.get_component<A>(e)->x << " ]" << std::endl;
        std::cout << " has B component [ " << world.get_component<B>(e)->c << " ]" << std::endl;
    }
};

void TEST_VIEW()
{
    for (size_t i = 0; i < 10; i++)
    {
        snek::entity e = world.spawn();
        world.initialize<C, D>(e, (i % 2 == 0 ? i + 1 : i - 1), i + 97);
    }

    auto group = world.view<C, D>();

    group.for_each([&](C &c, D &d)
                   { c.x = 1;
                d.c = 'a'; });

    group.for_each([&](const C &c, const D &d)
                   { std::cout << c.x << std::endl;
                    std::cout << d.c << std::endl; });
};

void TEST_REMOVE_COMPONENTS() {};
void TEST_KILL_ENTITY() {};
void TEST_IS_ALIVE() {};
