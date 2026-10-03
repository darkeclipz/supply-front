#include "sim/SimulationScheduler.hpp"

#include <stdexcept>

namespace sim {

void SimulationScheduler::advance(Simulation& simulation, std::chrono::nanoseconds elapsed, bool paused, int speed)
{
    if (elapsed < std::chrono::nanoseconds::zero()) {
        throw std::invalid_argument("elapsed time cannot be negative");
    }

    if (speed < 1 || speed > 16) {
        throw std::invalid_argument("speed must be between 1 and 16");
    }

    if (paused) {
        return;
    }

    constexpr auto tick_duration =
        std::chrono::nanoseconds{1'000'000'000 / ticks_per_second};

    m_accumulator += elapsed * speed;

    while (m_accumulator >= tick_duration) {
        simulation.tick();
        m_accumulator -= tick_duration;
    }
}

}
