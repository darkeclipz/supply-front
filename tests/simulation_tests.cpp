#include "sim/Simulation.hpp"
#include "sim/SimulationScheduler.hpp"
#include "sim/Command.hpp"
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

TEST_CASE("Speed changes preserve progress across pause", "[simulation][scheduler]") {
    using namespace std::chrono_literals;

    sim::Simulation simulation;
    sim::SimulationScheduler scheduler;

    scheduler.advance(simulation, 50ms);
    REQUIRE(simulation.current_tick() == 0);

    scheduler.advance(simulation, 5s, true, 4);
    REQUIRE(simulation.current_tick() == 0);

    scheduler.advance(simulation, 25ms, false, 2);
    REQUIRE(simulation.current_tick() == 1);

    scheduler.advance(simulation, 25ms, false, 4);
    REQUIRE(simulation.current_tick() == 2);
}

TEST_CASE("Scheduler validates speed bounds even while paused", "[simulation][scheduler]") {
    using namespace std::chrono_literals;

    sim::Simulation simulation;
    sim::SimulationScheduler scheduler;

    for (int speed : {-1, 0, 17}) {
        REQUIRE_THROWS_AS(
            scheduler.advance(simulation, 100ms, false, speed),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            scheduler.advance(simulation, 100ms, true, speed),
            std::invalid_argument);
    }
    REQUIRE(simulation.current_tick() == 0);

    scheduler.advance(simulation, 100ms, false, 16);
    REQUIRE(simulation.current_tick() == 16);
}

TEST_CASE("Frame partitioning preserves progress at each speed", "[simulation][scheduler]") {
    using namespace std::chrono_literals;

    for (int speed : {1, 2, 4}) {
        CAPTURE(speed);
        const auto tick_speed = static_cast<sim::Tick>(speed);

        sim::Simulation coarse_simulation;
        sim::Simulation fine_simulation;
        sim::SimulationScheduler coarse_scheduler;
        sim::SimulationScheduler fine_scheduler;

        coarse_scheduler.advance(
            coarse_simulation, 1015ms, false, speed);
        
        for (int frame = 0; frame < 203; ++frame) {
            fine_scheduler.advance(
                fine_simulation, 5ms, false, speed);
        }

        REQUIRE(coarse_simulation.current_tick() == sim::Tick{10} * tick_speed);
        REQUIRE(fine_simulation.current_tick() == sim::Tick{10} * tick_speed);

        coarse_scheduler.advance(coarse_simulation, 85ms, false, speed);
        fine_scheduler.advance(fine_simulation, 85ms, false, speed);

        REQUIRE(coarse_simulation.current_tick() == sim::Tick{11} * tick_speed);
        REQUIRE(fine_simulation.current_tick() == sim::Tick{11} * tick_speed);
    }
}

TEST_CASE("Gameplay IDs allocate deterministically", "[simulation][identity]") {
    sim::Simulation first;
    sim::Simulation second;

    for (std::uint64_t expected = 1; expected <= 3; ++expected) {
        const auto first_id = first.create_entity();
        const auto second_id = second.create_entity();
        
        REQUIRE(first_id.value == expected);
        REQUIRE(first_id == second_id);
    }
}

TEST_CASE("Simulation looks up stable gameplay IDs", "[simulation][identity]") {
    sim::Simulation simulation;

    REQUIRE_FALSE(simulation.entity_exists(sim::GameEntityId{}));
    REQUIRE_FALSE(simulation.entity_exists(sim::GameEntityId{999}));

    const auto first = simulation.create_entity();
    const auto second = simulation.create_entity();

    REQUIRE(simulation.entity_exists(first));
    REQUIRE(simulation.entity_exists(second));
    REQUIRE_FALSE(simulation.entity_exists(sim::GameEntityId{999}));
}

TEST_CASE("Destroyed gameplay IDs stay invalid", "[simulation][identity]") {
    sim::Simulation simulation;
    const auto first = simulation.create_entity();
    const auto second = simulation.create_entity();

    REQUIRE_FALSE(simulation.destroy_entity(sim::GameEntityId{}));
    REQUIRE_FALSE(simulation.destroy_entity(sim::GameEntityId{999}));

    REQUIRE(simulation.destroy_entity(first));
    REQUIRE_FALSE(simulation.entity_exists(first));
    REQUIRE(simulation.entity_exists(second));
    REQUIRE_FALSE(simulation.entity_exists(first));
    
    const auto replacement = simulation.create_entity();

    REQUIRE(replacement.value == 3);
    REQUIRE(simulation.entity_exists(replacement));
    REQUIRE_FALSE(simulation.entity_exists(first));
    REQUIRE_FALSE(simulation.destroy_entity(first));
    REQUIRE(simulation.entity_exists(replacement));
    REQUIRE(simulation.entity_exists(second));
}

TEST_CASE("Command submission validates tick and player sequence", "[simulation][commands]") {
    sim::Simulation simulation;
    const auto target = simulation.create_entity();
    using Result = sim::CommandSubmission;

    sim::Command command{
        .execute_at = 0,
        .player_id = 0,
        .sequence = 1,
        .payload = {target}
    };

    REQUIRE(simulation.submit_command(command) == Result::invalid_tick);

    command.execute_at = 1;
    command.sequence = 0;
    REQUIRE(simulation.submit_command(command) == Result::invalid_sequence);

    command.sequence = 1;
    REQUIRE(simulation.submit_command(command) == Result::queued);
    REQUIRE(simulation.entity_exists(target));
    REQUIRE(simulation.current_tick() == 0);

    REQUIRE(simulation.submit_command(command) == Result::invalid_sequence);

    command.sequence = 3;
    REQUIRE(simulation.submit_command(command) == Result::queued);

    command.sequence = 2;
    REQUIRE(simulation.submit_command(command) == Result::invalid_sequence);

    command.player_id = 1;
    command.sequence = 1;
    REQUIRE(simulation.submit_command(command) == Result::queued);
}