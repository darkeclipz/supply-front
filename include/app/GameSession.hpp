#pragma once

#include "sim/SimulationScheduler.hpp"
#include <chrono>

namespace app {

class GameSession {
public:
    void advance(std::chrono::nanoseconds elapsed, bool paused = false, int speed = 1);
    [[nodiscard]] sim::Tick current_tick() const noexcept;
private:
    sim::Simulation m_simulation;
    sim::SimulationScheduler m_scheduler;
};

}