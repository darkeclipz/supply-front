#pragma once

#include "sim/SimulationScheduler.hpp"
#include <chrono>

namespace app {

class GameSession {
public:
    void advance(std::chrono::nanoseconds elapsed, bool paused = false);
    [[no_discard]] sim::Tick current_tick() const noexcept;
private:
    sim::Simulation m_simulation;
    sim::SimulationScheduler m_scheduler;
};

}