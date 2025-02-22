#pragma once
#include "entity.hpp"
#include "unordered_map"
namespace snek
{
    class World
    {
        u64 world_mask;
        std::unordered_map<std::string, std::vector<Entity *>> entities_by_tag;
        std::unordered_map<u64, Entity *> entities_by_id;
        bool running;

    public:
        World() : world_mask(), entities_by_tag(), entities_by_id(), running(true) {};
        World(const World &o) : world_mask(o.world_mask), entities_by_tag(o.entities_by_tag), entities_by_id(o.entities_by_id), running(true) {};
        World(World &&o) : world_mask(o.world_mask), entities_by_tag(std::move(o.entities_by_tag)), entities_by_id(std::move(o.entities_by_id)), running(true) {};
        inline void WorldPause() { running = false; };
        Entity &spawn()
        {
            Entity *e = new Entity("");
            entities_by_tag[e->GetTag()].push_back(e);
            entities_by_id[e->GetID()] = e;
            world_mask |= e->GetID();
            return *e;
        };

        Entity &spawn(const std::string &tag)
        {
            Entity *e = new Entity(tag);
            entities_by_tag[e->GetTag()].push_back(e);
            entities_by_id[e->GetID()] = e;
            world_mask |= e->GetID();
            return *e;
        }

        [[nodsicard]] bool has_entity(Entity *e) const noexcept
        {
            return ((world_mask & e->GetID()) == e->GetID());
        };
        [[nodsicard]] bool has_entity(u64 id) const noexcept
        {
            return ((world_mask & id) == id);
        };
        [[nodiscard]] bool has_tag(const std::string &tag)
        {
            return (entities_by_tag.find(tag) != entities_by_tag.end());
        }

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