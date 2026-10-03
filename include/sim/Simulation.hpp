#pragma once

#include "sim/GameEntityId.hpp"

#include <cstdint>
#include <entt/entt.hpp>
#include <unordered_map>

namespace sim {
using Tick = std::uint64_t;
inline constexpr Tick ticks_per_second = 10;

class Simulation {
public:
    void tick();
    [[nodiscard]] Tick current_tick() const noexcept;
    [[nodiscard]] GameEntityId create_entity();
    [[nodiscard]] bool entity_exists(GameEntityId id) const;
private:
    entt::registry m_registry;
    Tick m_current_tick = 0;
    std::uint64_t m_last_entity_id = 0;
    std::unordered_map<std::uint64_t, entt::entity> m_entities_by_id;
};
}