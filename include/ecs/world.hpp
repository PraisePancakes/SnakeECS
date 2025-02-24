#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <map>
#include "snek_alloc.hpp"
#include <algorithm>

namespace snek
{
    template <typename SizeT = u64, typename Alloc = internal::snek_allocator<SizeT, Entity>>
    class World
    {
        using EntityMap = std::unordered_map<u64, Entity *>;
        using TagMap = std::unordered_map<std::string, std::vector<Entity *>>;
        EntityMap entities_by_id;
        TagMap entities_by_tag;
        bool running;

        Entity *create_entity(const std::string &tag)
        {
            Entity *e = new Entity("");
            entities_by_id.emplace(e->GetID(), e);
            entities_by_tag[tag].push_back(e);
            return e;
        }

    public:
        World() : entities_by_id(), entities_by_tag(), running(true) {};
        World(const World &o) : entities_by_id(o.entities_by_id), entities_by_tag(o.entities_by_tag), running(true) {};
        World(World &&o) : entities_by_id(std::move(o.entities_by_id)), entities_by_tag(std::move(o.entities_by_id)), running(true) {};
        inline void WorldPause() { running = false; };

        Entity &Spawn()
        {
            return *(create_entity(""));
        };
        Entity &Spawn(const std::string &tag)
        {
            return *(create_entity(tag));
        }
        Entity *GetEntityByID(u64 id) const noexcept
        {
            if (entities_by_id.find(id) == entities_by_id.end())
                return nullptr;
            return entities_by_id.at(id);
        };

        std::vector<Entity *> GetEntitiesByTag(const std::string &tag)
        {
            return entities_by_tag.at(tag);
        };
        // convert to variadic template to check if all entity ids exist, ensure argument set shares the same type
        bool HasEntity(const Entity &e) const noexcept
        {
            return entities_by_id.find(e.GetID()) != entities_by_id.end();
        };
        bool HasEntity(u64 id) const noexcept
        {
            return entities_by_id.find(id) != entities_by_id.end();
        };
        bool HasTag(const std::string &tag)
        {
            return entities_by_tag.at(tag).size() > 0;
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