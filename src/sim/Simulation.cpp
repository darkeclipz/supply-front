#include "sim/Simulation.hpp"

#include <limits>
#include <stdexcept>

namespace sim {

void Simulation::tick() {
    ++m_current_tick;
}

Tick Simulation::current_tick() const noexcept {
    return m_current_tick;
}

GameEntityId Simulation::create_entity() {
    if (m_last_entity_id == std::numeric_limits<std::uint64_t>::max()) {
        throw std::overflow_error("gameplay entity IDs exhausted");
    }

    const GameEntityId id{m_last_entity_id + 1};
    const auto entity = m_registry.create();

    try {
        m_registry.emplace<GameEntityId>(entity, id);
        m_entities_by_id.emplace(id.value, entity);
    } catch (...) {
        m_registry.destroy(entity);
        throw;
    }

    m_last_entity_id = id.value;
    return id;
}

bool Simulation::entity_exists(GameEntityId id) const {
    return m_entities_by_id.contains(id.value);
}

bool Simulation::destroy_entity(GameEntityId id) {
    const auto found = m_entities_by_id.find(id.value);
    if (found == m_entities_by_id.end()) {
        return false;
    }

    m_registry.destroy(found->second);
    m_entities_by_id.erase(found);
    return true;
}

CommandSubmission Simulation::submit_command(Command command) {
    if (command.execute_at <= m_current_tick) {
        return CommandSubmission::invalid_tick;
    }

    const auto entry = m_last_command_sequence.try_emplace(command.player_id, 0).first;

    if (command.sequence <= entry->second) {
        return CommandSubmission::invalid_sequence;
    }

    m_pending_commands.push_back(command);
    entry->second = command.sequence;

    return CommandSubmission::queued;
}

}