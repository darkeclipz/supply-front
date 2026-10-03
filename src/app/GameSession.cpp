#include "app/GameSession.hpp"

namespace app {

void GameSession::advance(std::chrono::nanoseconds elapsed, bool paused) {
    m_scheduler.advance(m_simulation, elapsed, paused);
}

sim::Tick GameSession::current_tick() const noexcept {
    return m_simulation.current_tick();
}

}