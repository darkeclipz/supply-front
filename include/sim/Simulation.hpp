#pragma once

#include "sim/GameEntityId.hpp"

#include <cstdint>
#include <entt/entt.hpp>

namespace sim {
using Tick = std::uint64_t;
inline constexpr Tick ticks_per_second = 10;

class Simulation {
public:
    void tick();
    [[nodiscard]] Tick current_tick() const noexcept;
private:
    entt::registry m_registry;
    Tick m_current_tick = 0;
};
}