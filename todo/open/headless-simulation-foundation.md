# Establish the headless simulation foundation

## Description

Introduce a gameplay simulation module using the separation in docs/EXAMPLE-DESIGN.md, adapted to the overhead 3D, proposed 10 Hz architecture in docs/SYSTEM-DESIGN.md. Retain the existing include/src split and top-level tests directory. Add folders as their first implementation arrives. Start with integer tick progression; then add a separately testable frame-time scheduler and connect it to the application.

Tutor checkpoint: active.

- Current step: Counter initialization verified. Scheduler partially started: header declares advance(), but m_accumulator[0] is a zero-length array and the source is empty. Complete the basic 10 Hz accumulator; correction and implementation proposed.
- Context: User chose snake_case methods/constants and m_ member prefixes: current_tick(), ticks_per_second, m_registry, m_current_tick. Preserve this convention. Frame timing belongs outside Simulation. Existing engine::fixedUpdate only spins demo models; do not treat it as gameplay simulation.
- Verification: Source inspection confirms Tick m_current_tick = 0;. On resumption, cmake --build --preset debug --target scene_tests simulation_tests -j 2 succeeded (no work to do), and ctest --preset debug passed all 13 tests. Scheduler checks have not run because it is not implemented.
- Next action: The programmer will replace m_accumulator[0] with m_accumulator{0}, implement advance() retaining fractional tick time and rejecting negative durations, add its source to supply_sim, and add remainder and negative-duration tests in tests/simulation_tests.cpp. Rebuild both test targets and run ctest --preset debug; expect 15 passing tests. Review before adding pause and partition-equivalence tests.
- Blocker: None. Use integer chrono durations for the scheduler; Simulation remains independent of frame time. Pause/speed behavior and application integration remain later steps.

## Acceptance criteria

- [x] A separate supply_sim library owns simulation state and builds without graphics dependencies.
- [x] Simulation starts at tick zero, increments once per tick(), and passes a repeated-tick test.
- [ ] A frame-time scheduler advances fixed 10 Hz ticks with tested remainder and pause behavior.
- [ ] Equal elapsed time partitioned into different frame durations produces equal tick counts in scheduler tests.
- [ ] The application uses the simulation scheduler independently of rendering, and existing scene tests still pass.
