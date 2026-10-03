# Establish the headless simulation foundation

## Description

Introduce a gameplay simulation module using the separation in docs/EXAMPLE-DESIGN.md, adapted to the overhead 3D, proposed 10 Hz architecture in docs/SYSTEM-DESIGN.md. Retain the existing include/src split and top-level tests directory. Add folders as their first implementation arrives. Start with integer tick progression; then add a separately testable frame-time scheduler and connect it to the application.

Tutor checkpoint: active.

- Current step: Pause and partition-equivalence tests verified. Connect SimulationScheduler to src/main.cpp and show its tick counter; proposed, not implemented.
- Context: User chose snake_case methods/constants and m_ member prefixes: current_tick(), ticks_per_second, m_registry, m_current_tick. Preserve this convention. Frame timing belongs outside Simulation. Existing engine::fixedUpdate only spins demo models; do not treat it as gameplay simulation.
- Verification: Header default argument confirmed. cmake --build --preset debug --target scene_tests simulation_tests -j 2 succeeded; ctest --preset debug passed all 17 tests, including pause/remainder preservation and equal tick progression under differently partitioned elapsed time. Application integration has not been verified.
- Next action: The programmer will link sandbox to supply_sim, include SimulationScheduler.hpp and chrono in src/main.cpp, create a simulation and scheduler in run(), read frame time once, convert its uncapped nonnegative duration to nanoseconds, call advance(..., !playing) after editor input, and draw a tick label. Preserve the template's separate 60 Hz demo animation for now. Build Debug, run tests, and manually check roughly 10 ticks/sec and pause/resume in the sandbox.
- Blocker: None. Chrono conversion truncates sub-nanosecond fractions; the partition test proves behavior for exact integer durations, not cross-machine raylib wall-clock equivalence. Speed controls and GameSession extraction are later work.

## Acceptance criteria

- [x] A separate supply_sim library owns simulation state and builds without graphics dependencies.
- [x] Simulation starts at tick zero, increments once per tick(), and passes a repeated-tick test.
- [x] A frame-time scheduler advances fixed 10 Hz ticks with tested remainder and pause behavior.
- [x] Equal elapsed time partitioned into different frame durations produces equal tick counts in scheduler tests.
- [ ] The application uses the simulation scheduler independently of rendering, and existing scene tests still pass.
