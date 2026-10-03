# Establish the headless simulation foundation

## Description

Introduce a gameplay simulation module using the separation in docs/EXAMPLE-DESIGN.md, adapted to the overhead 3D, proposed 10 Hz architecture in docs/SYSTEM-DESIGN.md. Retain the existing include/src split and top-level tests directory. Add folders as their first implementation arrives. Start with integer tick progression; then add a separately testable frame-time scheduler and connect it to the application.

- Final result: Headless simulation, 10 Hz scheduler, pause/remainder handling, partition-equivalence tests, and sandbox integration verified.
- Context: User chose snake_case methods/constants and m_ member prefixes: current_tick(), ticks_per_second, m_registry, m_current_tick. Preserve this convention. Frame timing belongs outside Simulation. Existing engine::fixedUpdate only spins demo models; do not treat it as gameplay simulation.
- Verification: Source confirms sandbox links supply_sim, reads frame time once, converts uncapped nonnegative seconds to nanoseconds, advances after editor input using !playing, and draws current_tick(). cmake --build --preset debug -j 2 succeeded; ctest --preset debug passed all 17 tests. timeout 15s ./build/debug/bin/sandbox --smoke-test failed at window initialization because GLFW could not open X11 display :0; no visual behavior was verified here.
- Desktop verification: The programmer reports that the counter advances roughly 10 ticks/sec, freezes while paused, and continues when resumed. This completes the visual verification unavailable in the agent environment.
- Next action: Continue with todo/open/extract-game-session.md. Chrono conversion truncates sub-nanosecond fractions; exact-duration partition tests do not establish cross-machine raylib wall-clock equivalence. Speed controls remain later work.

## Acceptance criteria

- [x] A separate supply_sim library owns simulation state and builds without graphics dependencies.
- [x] Simulation starts at tick zero, increments once per tick(), and passes a repeated-tick test.
- [x] A frame-time scheduler advances fixed 10 Hz ticks with tested remainder and pause behavior.
- [x] Equal elapsed time partitioned into different frame durations produces equal tick counts in scheduler tests.
- [x] The application uses the simulation scheduler independently of rendering, and existing scene tests still pass.
