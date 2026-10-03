#pragma once

#include "sim/Simulation.hpp"
#include <chrono>

namespace sim {

class SimulationScheduler {
public:
    void advance(Simulation& simulation, 
        std::chrono::nanoseconds elapsed, 
        bool paused = false, 
        int speed = 1);

private:
    std::chrono::nanoseconds m_accumulator{0};
};

}