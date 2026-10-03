#include "sim/Simulation.hpp"
#include "sim/SimulationScheduler.hpp"
#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <stdexcept>

TEST_CASE("Simulation starts at tick zero", "[simulation]") {
    sim::Simulation simulation;
    REQUIRE(simulation.current_tick() == 0);
}

TEST_CASE("One simulation step advances one tick", "[simulation]") {
    sim::Simulation simulation;
    simulation.tick();
    REQUIRE(simulation.current_tick() == 1);
}

TEST_CASE("Simulation tracks repeated steps", "[simulation]") {
    sim::Simulation simulation;
    for (int i = 0; i < 100; ++i) {
        simulation.tick();
    }
    REQUIRE(simulation.current_tick() == 100);
}

TEST_CASE("Scheduler retains time between frames", "[simulation]") {
    using namespace std::chrono_literals;

    sim::Simulation simulation;
    sim::SimulationScheduler scheduler;

    scheduler.advance(simulation, 250ms);
    REQUIRE(simulation.current_tick() == 2);

    scheduler.advance(simulation, 50ms);
    REQUIRE(simulation.current_tick() == 3);
}

TEST_CASE("Scheduler rejects negative elapsed time", "[simulation]") {
    using namespace std::chrono_literals;

    sim::Simulation simulation;
    sim::SimulationScheduler scheduler;

    REQUIRE_THROWS_AS(
        scheduler.advance(simulation, -1ms),
        std::invalid_argument);
    REQUIRE(simulation.current_tick() == 0);
}

TEST_CASE("Paused time is ignored and partial progress is preserved", "[simulation][scheduler]") {
    using namespace std::chrono_literals;

    sim::Simulation simulation;
    sim::SimulationScheduler scheduler;

    scheduler.advance(simulation, 50ms);
    REQUIRE(simulation.current_tick() == 0);

    scheduler.advance(simulation, 5s, true);
    REQUIRE(simulation.current_tick() == 0);

    scheduler.advance(simulation, 50ms);
    REQUIRE(simulation.current_tick() == 1);
}

TEST_CASE("Frame duration partitioning preserve", "[simulation][scheduler]") {
    using namespace std::chrono_literals;

    sim::Simulation coarse_simulation;
    sim::Simulation fine_simulation;
    sim::SimulationScheduler coarse_scheduler;
    sim::SimulationScheduler fine_scheduler;

    coarse_scheduler.advance(coarse_simulation, 1050ms);

    for (int i = 0; i < 70; ++i) {
        fine_scheduler.advance(fine_simulation, 15ms);
    }

    REQUIRE(coarse_simulation.current_tick() == 10);
    REQUIRE(fine_simulation.current_tick() == 10);

    // Both schedules should also retain the 50 ms remainder.

    coarse_scheduler.advance(coarse_simulation, 50ms);
    fine_scheduler.advance(fine_simulation, 50ms);

    REQUIRE(coarse_simulation.current_tick() == 11);
    REQUIRE(fine_simulation.current_tick() == 11);
}

TEST_CASE("Double speed preserves partial tick progress", "[simulation][scheduler]") {
    using namespace std::chrono_literals;

    sim::Simulation simulation;
    sim::SimulationScheduler scheduler;

    scheduler.advance(simulation, 25ms, false, 2);
    REQUIRE(simulation.current_tick() == 0);

    scheduler.advance(simulation, 25ms, false, 2);
    REQUIRE(simulation.current_tick() == 1);
}
