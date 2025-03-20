#pragma once

namespace snek
{
    namespace uuid
    {

        template <typename WorldPolicy>
        struct identifier
        {
            static inline auto generate_entity_id() noexcept
            {
                static typename WorldPolicy::entity_index_t new_id = 0;
                typename WorldPolicy::entity_index_t old = new_id;
                new_id++;
                return old;
            };
        };

    }

}