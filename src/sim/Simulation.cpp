#include "sim/Simulation.hpp"

#include <climits>
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
    } catch (...) {
        m_registry.destroy(entity);
        throw;
    }

    m_last_entity_id = id.value;
    return id;
}

}