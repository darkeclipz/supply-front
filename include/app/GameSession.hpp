#pragma once

#include "sim/SimulationScheduler.hpp"
#include "sim/Command.hpp"
#include "sim/GameEntityId.hpp"
#include "sim/Position.hpp"
#include <chrono>
#include <vector>
#include <optional>

namespace app {

class GameSession {
public:
    GameSession();
    void advance(std::chrono::nanoseconds elapsed, bool paused = false, int speed = 1);
    [[nodiscard]] sim::Tick current_tick() const noexcept;
    [[nodiscard]] sim::CommandSubmission submit_command(sim::Command command);
    [[nodiscard]] const std::vector<sim::CommandOutcome>& command_outcomes() const noexcept;
    [[nodiscard]] sim::GameEntityId prototype_entity() const noexcept;
    [[nodiscard]] std::optional<sim::Position> position(sim::GameEntityId id) const;
private:
    sim::Simulation m_simulation;
    sim::SimulationScheduler m_scheduler;
    sim::GameEntityId m_prototype_entity{};
};

}