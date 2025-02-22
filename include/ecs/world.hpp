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
        World() : world_mask(0), entities_by_tag(), entities_by_id(), running(true) {};
        World(const World &o) : world_mask(o.world_mask), entities_by_tag(o.entities_by_tag), entities_by_id(o.entities_by_id), running(true) {};
        World(World &&o) : world_mask(o.world_mask), entities_by_tag(std::move(o.entities_by_tag)), entities_by_id(std::move(o.entities_by_id)), running(true) {};

        inline void WorldPause() { running = false; };
        Entity &Spawn()
        {
            Entity *e = new Entity("");
            entities_by_tag[e->GetTag()].push_back(e);
            entities_by_id[e->GetID()] = e;
            world_mask |= e->GetID();
            return *e;
        };
        Entity &Spawn(const std::string &tag)
        {
            Entity *e = new Entity(tag);
            entities_by_tag[e->GetTag()].push_back(e);
            entities_by_id[e->GetID()] = e;
            world_mask |= e->GetID();
            return *e;
        }
        Entity *GetEntityByID(u64 id) const noexcept
        {
            if (entities_by_id.find(id) == entities_by_id.end())
                return nullptr;
            return entities_by_id.at(id);
        };
        [[nodiscard]] std::vector<Entity *> GetEntitiesByTag(const std::string &tag) const
        {
            return entities_by_tag.at(tag);
        }
        bool HasEntity(const Entity &e) const noexcept
        {
            return ((world_mask & e.GetID()) == e.GetID());
        };
        bool HasEntity(u64 id) const noexcept
        {
            return ((world_mask & id) == id);
        };
        bool HasTag(const std::string &tag)
        {
            return (entities_by_tag.find(tag) != entities_by_tag.end());
        }
        [[nodiscard]] bool IsRunning() const
        {
            return this->running;
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