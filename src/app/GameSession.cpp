#include "app/GameSession.hpp"

namespace app {

GameSession::GameSession() {
    m_prototype_entity = m_simulation.create_entity(sim::Position{2000, 1000});
}

sim::GameEntityId GameSession::prototype_entity() const noexcept {
    return m_prototype_entity;
}

std::optional<sim::Position> GameSession::position(sim::GameEntityId id) const {
    return m_simulation.position(id);
}

void GameSession::advance(std::chrono::nanoseconds elapsed, 
                          bool paused, 
                          int speed) {
    m_scheduler.advance(m_simulation, elapsed, paused, speed);
}

sim::Tick GameSession::current_tick() const noexcept {
    return m_simulation.current_tick();
}

sim::CommandSubmission GameSession::submit_command(sim::Command command) {
    return m_simulation.submit_command(command);
}

const std::vector<sim::CommandOutcome>& GameSession::command_outcomes() const noexcept {
    return m_simulation.command_outcomes();
}

}