#include <iostream>
#include "../include/ecs/entity_traits.hpp"
#include "../include/ecs/entity.hpp"
#include "../include/ecs/world.hpp"

using namespace snek;
using namespace snek::internal;

// e.AddComponent<A, B, C>("a", "b", "c");

class A : public Component
{
public:
    std::string str;
    A(std::string s) : str(s) {

                       };
};
class B : public Component
{
public:
    B(std::string s) {
    };
};

int main(int argc, char **argv)
{
    World w;
    Entity e = w.Spawn();
    e.InitializeComponents<A, B>("A", "B");
    std::cout << e.GetComponent<A>()->str;

    return 0;
}