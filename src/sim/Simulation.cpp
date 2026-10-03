#include "sim/Simulation.hpp"

namespace sim {

void Simulation::tick() {
    ++m_current_tick;
}

Tick Simulation::current_tick() const noexcept {
    return m_current_tick;
}

}