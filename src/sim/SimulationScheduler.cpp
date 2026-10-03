#include "sim/SimulationScheduler.hpp"

#include <stdexcept>

namespace sim {

void SimulationScheduler::advance(
    Simulation& simulation,
    std::chrono::nanoseconds elapsed)
{
    if (elapsed < std::chrono::nanoseconds::zero()) {
        throw std::invalid_argument("elapsed time cannot be negative");
    }

    constexpr auto tick_duration =
        std::chrono::nanoseconds{1'000'000'000 / ticks_per_second};

    m_accumulator += elapsed;

    while (m_accumulator >= tick_duration) {
        simulation.tick();
        m_accumulator -= tick_duration;
    }
}

}
