#include <iostream>
#include "../include/ecs/entity.hpp"
#include "../include/ecs/component.hpp"
#include "../include/ecs/entity.hpp"

using namespace snek;
using namespace core;

class B : public Component
{

public:
    int x = 0;
    B(int x) : x(x) {};
    ~B() {};
};

int main(int argc, char **argv)
{
    Entity e;

    B b = e.AddComponent<B>(3);
    std::cout << std::boolalpha << e.HasComponent<B>() << std::endl;
    e.RemoveComponent<B>();
    std::cout << std::boolalpha << e.HasComponent<B>() << std::endl;
    return 0;
}