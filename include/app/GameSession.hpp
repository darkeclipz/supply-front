#pragma once

#include "sim/SimulationScheduler.hpp"
#include "sim/Command.hpp"
#include <chrono>
#include <vector>

namespace app {

class GameSession {
public:
    void advance(std::chrono::nanoseconds elapsed, bool paused = false, int speed = 1);
    [[nodiscard]] sim::Tick current_tick() const noexcept;
    [[nodiscard]] sim::CommandSubmission submit_command(sim::Command command);
    [[nodiscard]] const std::vector<sim::CommandOutcome>& command_outcomes() const noexcept;
private:
    sim::Simulation m_simulation;
    sim::SimulationScheduler m_scheduler;
};

}