#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <map>
#include "snek_alloc.hpp"
#include <algorithm>

namespace snek
{
    template <typename T, u64 n, typename Alloc = internal::snek_linear_allocator<T>, typename = std::enable_if_t<std::is_base_of_v<Entity, T>>>
    class World
    {
    public:
        using alloc_traits = std::allocator_traits<Alloc>;

        // Entity* will store pointer to constructed type from the allocator which holds the object in preallocated memory.
        using EntityMap = std::unordered_map<u64, typename alloc_traits::pointer>;
        using TagMap = std::unordered_map<std::string, std::vector<typename alloc_traits::pointer>>;

    private:
        Alloc _alloc;
        EntityMap entities_by_id;
        TagMap entities_by_tag;
        static constexpr u64 max_size = n;
        bool running;
        alloc_traits::pointer _region;

        [[nodiscard]] typename alloc_traits::pointer create_entity(const std::string &tag)
        {
            try
            {
                typename alloc_traits::pointer p = _alloc.construct(tag);
                if (!p)
                {
                    throw std::runtime_error("construction overflowed maximum allowed world storage [max = " + std::to_string(max_size) + "]");
                }
                entities_by_id.emplace(p->GetID(), p);
                entities_by_tag[tag].push_back(p);
                return p;
            }
            catch (std::exception &e)
            {
                std::cerr << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }
        }

    public:
        World()
            : entities_by_id(),
              entities_by_tag(),
              running(true),
              _region(alloc_traits::allocate(_alloc, n)) {
              };

        World(const World &o)
            : entities_by_id(o.entities_by_id),
              entities_by_tag(o.entities_by_tag),
              running(true),
              _region(o.entity_region) {};

        World(World &&o)
            : entities_by_id(std::move(o.entities_by_id)),
              entities_by_tag(std::move(o.entities_by_id)),
              _region(std::move(o._region)),
              running(true) {};

        inline void WorldPause() { running = false; };

        [[nodiscard]] alloc_traits::value_type &Spawn()
        {
            return *(create_entity(""));
        };
        [[nodiscard]] alloc_traits::value_type &Spawn(const std::string &tag)
        {
            return *(create_entity(tag));
        }
        [[nodiscard]] alloc_traits::pointer GetEntityByID(u64 id) const noexcept
        {
            if (entities_by_id.find(id) == entities_by_id.end())
                return nullptr;
            return entities_by_id.at(id);
        };
        [[nodiscard]] Alloc &GetWorldAllocator() const noexcept { return _alloc; };
        [[nodiscard]] std::vector<typename alloc_traits::pointer> GetEntitiesByTag(const std::string &tag)
        {
            return entities_by_tag.at(tag);
        };
        // convert to variadic template to check if all entity ids exist, ensure argument set shares the same type
        [[nodiscard]] bool HasEntity(const alloc_traits::value_type &e) const noexcept
        {
            return entities_by_id.find(e.GetID()) != entities_by_id.end();
        };
        [[nodiscard]] bool HasEntity(u64 id) const noexcept
        {
            return entities_by_id.find(id) != entities_by_id.end();
        };
        [[nodiscard]] bool HasTag(const std::string &tag)
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