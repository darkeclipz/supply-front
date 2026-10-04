#include "sim/Simulation.hpp"

#include <limits>
#include <stdexcept>
#include <algorithm>
#include <tuple>

namespace sim {

void Simulation::tick() {
    if (m_current_tick == std::numeric_limits<Tick>::max()) {
        throw std::overflow_error("simulation tick exhausted");
    }

    const Tick next_tick = m_current_tick + 1;

    std::sort(m_pending_commands.begin(), m_pending_commands.end(),
        [](const Command& a, const Command& b) {
            return std::tie(a.execute_at, a.player_id, a.sequence)
                <  std::tie(b.execute_at, b.player_id, b.sequence);
        });

    const auto due_end = std::find_if(
        m_pending_commands.begin(), m_pending_commands.end(),
        [next_tick](const Command& command) {
            return command.execute_at > next_tick;
        });
    
    const auto due_count = static_cast<std::size_t>(
        due_end - m_pending_commands.begin());

    m_command_outcomes.reserve(m_command_outcomes.size() + due_count);

    for (auto entry = m_pending_commands.begin(); entry != due_end; ++entry) {
        const auto result = destroy_entity(entry->payload.target)
            ? CommandExecution::accepted
            : CommandExecution::missing_entity;
        
        m_command_outcomes.push_back(CommandOutcome{*entry, result});
    }

    m_pending_commands.erase(m_pending_commands.begin(), due_end);
    m_current_tick = next_tick;
}

Tick Simulation::current_tick() const noexcept {
    return m_current_tick;
}

GameEntityId Simulation::create_entity(Position position) {
    if (m_last_entity_id == std::numeric_limits<std::uint64_t>::max()) {
        throw std::overflow_error("gameplay entity IDs exhausted");
    }

    const GameEntityId id{m_last_entity_id + 1};
    const auto entity = m_registry.create();

    try {
        m_registry.emplace<GameEntityId>(entity, id);
        m_registry.emplace<Position>(entity, position);
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

std::optional<Position> Simulation::position(GameEntityId id) const {
    const auto found = m_entities_by_id.find(id.value);
    if (found == m_entities_by_id.end()) {
        return std::nullopt;
    }

    return m_registry.get<Position>(found->second);
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

const std::vector<CommandOutcome>& Simulation::command_outcomes() const noexcept {
    return m_command_outcomes;
}

}