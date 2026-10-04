#include "app/GameSession.hpp"

namespace app {

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