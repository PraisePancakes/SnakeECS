#pragma once
#include "entity.hpp"
#include "unordered_map"
namespace snek
{
    class World
    {
        u64 world_mask;
        std::unordered_map<std::string, std::vector<Entity *>> entities;
        bool running;

    public:
        World() : world_mask(), entities(), running(true) {};
        World(const World &o) : world_mask(o.world_mask), entities(o.entities), running(true) {};
        World(World &&o) : world_mask(o.world_mask), entities(std::move(o.entities)), running(true) {};
        inline void WorldPause() { running = false; };
        
        ~World() {

        };
        // add entity
        // remove entity
        // add_entity(tag)
        // get_entity(id)
        // has_entity(id)
        // get_entities(key) -> vector
    };
};