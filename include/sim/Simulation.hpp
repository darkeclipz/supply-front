#pragma once

#include "sim/GameEntityId.hpp"
#include "sim/Tick.hpp"
#include "sim/Command.hpp"

#include <cstdint>
#include <entt/entt.hpp>
#include <unordered_map>
#include <vector>

namespace sim {

class Simulation {
public:
    void tick();
    [[nodiscard]] Tick current_tick() const noexcept;
    [[nodiscard]] GameEntityId create_entity();
    [[nodiscard]] bool entity_exists(GameEntityId id) const;
    [[nodiscard]] bool destroy_entity(GameEntityId id);
    [[nodiscard]] CommandSubmission submit_command(Command command);
private:
    entt::registry m_registry;
    Tick m_current_tick = 0;
    std::uint64_t m_last_entity_id = 0;
    std::unordered_map<std::uint64_t, entt::entity> m_entities_by_id;
    std::vector<Command> m_pending_commands;
    std::unordered_map<std::uint32_t, std::uint64_t> m_last_command_sequence;
};
}