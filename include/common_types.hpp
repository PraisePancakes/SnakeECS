#pragma once
#include <iostream>
#include <cstdint>

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;

namespace snek
{
    using entity = u64;

    using component_id = std::uint64_t;
    using component_mask = std::uint64_t;
}
