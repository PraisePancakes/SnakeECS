# SnakeECS : Rtti-free, Policy-based ECS framework.
## **_Introduction_**
An Entity Component System is an architectural design pattern aimed at performing operations within a system in a cache-friendly manner.
There is too much to get into with this design pattern, so check out more information on its usage and concept [here](https://skypjack.github.io/2019-02-14-ecs-baf-part-1/).


## **_Motivation_**
### **_Rtti-free_** 
Entity-Component-Systems are designed with performance in mind, especially for real-time applications where speed and efficiency are critical. Incorporating run-time type information (RTTI) introduces unnecessary overhead, increases binary size, and can hinder performance in time-sensitive scenarios. I eliminated RTTI from the framework to maintain a lean, fast, and predictable system. This decision ensures that the ECS remains lightweight and optimized, avoiding runtime costs and enabling better control over memory and execution flow.

### **_Policy-based design_**

#### @Snek [configuration policy]
A configuration policy is just a configuration for your world. The policy takes three template arguments.

- entity type    : an integral type (u32, u64) for your entity
- component list : a type list of all the components needed for your world
- world allocator: how the internal storage of this world should be allocated.
```c++
//assuming we have this set of components (a, b, c, d)
using component_types = snek::component_list<component_a, component_b, component_c, component_d>;
using configuration_policy = snek::world_policy<std::uint64_t, component_types, std::allocator<std::uint64_t>>;
```
Since our components are explicitly injected via the policy, we don't have to worry about any runtime overhead with dynamic component types.

## **_Usage/Examples_**

```C++
 #include "snakeecs.hpp"

    struct A
    {
        int x;
        A(int x) : x(x) {};
        ~A() {};
    };

    struct B
    {
        char x;
        B(char x) : x(x) {};
        ~B() {};
    };

    using component_types = snek::component_list<A, B>;
    using configuration_policy = snek::world_policy<std::uint64_t, component_types, std::allocator<std::uint64_t>>;

    // systematic view
    void update(snek::world<configuration_policy>& w) {
        auto view = w.view<A, B>();
        
        view.for_each([](const A& a, const B& b) {
            int ax = a.x;
            char bx = b.x;
            std::cout << ax << " : " << bx << std::endl;
        });
    }

    int main(int argc, char** argv) {
        /*
            Let's begin by instantiating a world for our ECS. 
            Remember the configuration policy we created (See above)?
            Let's inject it into our world.
        */

        snek::world<configuration_policy> w;

        for(int i = 0; i < 10; i++) {
            auto e = w.spawn();
            //bind one component
            w.bind<A>(e, 5);
        }

        //or initialize

        for(int i = 0; i < 10; i++) {
            auto e = w.spawn();
            //initialize multiple components
            w.initialize<A, B>(e, 5, 'B');
        }
        return 0;
    }
```


## Build

This project is built using CMake (Version 3.28) with ISO C++20 Standard.
To run existing tests, from the root folder input these commands if not done already.
```bash
  mkdir build 
  cd build
  cmake -G "<Preferred-Generator>" ..
  cmake --build .
  ./SnakeECS
```
    
## Acknowledgements
I'd like to thank the following developers for their inspiration for different subsets of this project...
 - [Skypjack's EnTT](https://github.com/skypjack/entt)
 - [Chrischristakis' seecs](https://github.com/chrischristakis/)
 - [Vittorio Romeo's ECS Talk CppCon2015](https://www.youtube.com/watch?v=NTWSeQtHZ9M&t=2809s)


## Authors

- [@Praisepancakes](https://github.com/PraisePancakes)
