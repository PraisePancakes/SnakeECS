# SnakeECS : Rtti-free, Policy-based ECS framework.
## **_Introduction_**
An Entity Component System is an architectural design pattern aimed at performing operations within a system in a cache-friendly manner.
There is too much to get into with this design pattern, so check out more information on its usage and concept [here](https://skypjack.github.io/2019-02-14-ecs-baf-part-1/).


## **_Motivation_**
### **_Rtti-free_** 
Entity-Component-Systems are designed with performance in mind, especially for real-time applications where speed and efficiency are critical. Incorporating run-time type information (RTTI) introduces unnecessary overhead, increases binary size, and can hinder performance in time-sensitive scenarios. I eliminated RTTI from the framework to maintain a lean, fast, and predictable system. This decision ensures that the ECS remains lightweight and optimized, avoiding runtime costs and enabling better control over memory and execution flow.

### **_Policy-based design_**

#### @Snek [configuration policy]
A configuration policy is just a configuration for your world. The policy is composed of 5 different template parameters.

- entity type     : an integral type (u32, u64) for your entity
- component list  : a type list of all the components needed for your world
- world allocator : how the internal storage of this world should be allocated. (defaulted to std::allocator<entity_type>)
- tag type        : an enumerable list of constants for different entity tags 
- policy tag type : a tag that distinguishes this policy from others (defaulted to snek::snek_main_policy_tag)
```c++
//assuming we have this set of components (a, b, c, d)
using component_types = snek::component_list<component_a, component_b, component_c, component_d>;
using configuration_policy = snek::world_policy<std::uint64_t, component_types, std::allocator<std::uint64_t>>;
```
Since our components are explicitly injected via the policy, we don't have to worry about any runtime overhead with dynamic component types.
### **_Unified Policy Systems_**
The motive of the policy-based design allows for static unified systems based on policy constraints. Snek utilizes multi-world applications, alluding to the separation of concerns of each world.
For instance, say we have two unique states of our application, Game state and Menu state. Each state has its world of entities that don't necessarily relate to the other's entities but may implement shared components differently.
```c++
snek::world<menu_configuration_policy> menu_world;
``` 
and 
```c++
snek::world<game_configuration_policy> game_world;
```
Let's say both states incorporate a particle system, but the game state may incorporate particles relative to rigid bodies.
```c++
using game_component_types = snek::component_list<particle, rigidbody>;
using game_configuration_policy = snek::world_policy<std::uint64_t, game_component_types, std::allocator<std::uint64_t>>;

using menu_component_types = snek::component_list<particle>;
using menu_configuration_policy = snek::world_policy<std::uint64_t, menu_component_types, std::allocator<std::uint64_t>>;

```
We can create one system that handles both of these cases through static policy constraints :
```c++

template<typename Policy>
class ParticleSystem {
  snek::world<Policy>& particle_world;

void update_particle_with_rigidbody() {
 std::cout << "updating particles relative to rigidbody" << std::endl;
};
void update_menu_particles() {
 std::cout << "updating particles in menu" << std::endl;
};

public:
 ParticleSystem(snek::world<Policy>& w) : particle_world(w) {};

void update(float dt) {
//game policy constraint
 if constexpr(Policy::is_valid_component_set<particle, rigidbody>()) {
  update_particle_with_rigidbody();
 }
//constraint menu policy constraint
 if constexpr(Policy::is_valid_component<particle>()) {
  update_menu_particles();
 }
}
 ~ParicleSystem() {};  

}
```
However, there is one problem: what if both worlds share the same set of components...?

### **_Policy Tags_**
The world policy takes a policy tag as its final template argument (defaulted to snek::snek_main_policy_tag). This argument must derive from snek::policy_tag
```c++
  struct game_policy_tag : public snek::policy_tag {};
  struct menu_policy_tag : public snek::policy_tag {};
```
and may be used to distinguish different policies in our unified system.

```c++
 if constexpr(Policy::is_valid_component_set<particle, rigidbody>() && Policy::is_tagged<menu_policy_tag>()) {
   //handle menu particle system
 }
 if constexpr(Policy::is_valid_component_set<particle, rigidbody>() && Policy::is_tagged<game_policy_tag>()) {
   //handle game particle system
 }
```

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
### **_Entity Representation_**
In SnakeECS, entities are represented via either a 32-bit or 64-bit number. 
These numbers represent two common ideas.
1. Indexing
2. Versioning

The lower 8 bits of each entity ID are reserved strictly for versioning. This means each entity has a max version of 255, this version does not wrap to 0. 255 will be the last version of the entity, no matter what.
The remaining higher bits (depending on whether 32-bit or 64-bit) are reserved strictly for indexing the entity. 
For example, when you call,

```C++
auto entity = world.spawn();
//if entity = 1 its representation (assuming 32-bit) -> 0000 0000 0000 0000 0000 0001   0000 0000
//                                                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ : ^^^^^^^^^
//                                                        index representation          version representation
```
an index is returned to the caller. SnakeECS bases all API calls on indexing rather than arbitrary IDs. The library handles IDs internally.
To retrieve the version of an entity, you may call :
```C++
auto version = world.to_version(entity);
```
versions are incremented on each "removal".
```C++
 world.kill(entity); // increments version
```

## Build

This project uses CMake (Version 3.28) with the ISO C++20 Standard.
To run existing tests, from the root folder, input these commands if not done already.
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
