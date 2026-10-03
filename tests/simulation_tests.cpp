#include "sim/Simulation.hpp"
#include "sim/SimulationScheduler.hpp"
#include <catch2/catch_test_macros.hpp>

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