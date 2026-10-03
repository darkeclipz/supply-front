# Extract game session ownership

## Description

Introduce the app/GameSession boundary from docs/EXAMPLE-DESIGN.md. GameSession owns Simulation and SimulationScheduler; main.cpp retains the window, input, frame-time conversion, and rendering. Preserve the existing include/src split, snake_case methods, and m_ member prefixes. The temporary engine demo animation remains separate.

Tutor checkpoint: active.

- Current step: Add include/app/GameSession.hpp and src/app/GameSession.cpp, a headless supply_session library, and replace main.cpp's simulation/scheduler locals with one session; proposed, not implemented.
- Context: Expose advance(nanoseconds, bool paused = false) and current_tick() const. Keep simulation mutation private. Current pause choice stays in the existing playing UI variable for this small extraction.
- Verification: Prior foundation passes the Debug build and all 17 tests; programmer reports desktop 10 Hz/pause/resume checks pass. No checks run for this extraction.
- Next action: The programmer will implement the class and CMake wiring shown in chat, delegate advancement and tick display through session, build Debug, run all tests, and verify the visible counter/pause behavior is unchanged.
- Blocker: None. Speed controls, commands, and GameApp extraction are subsequent work.

## Acceptance criteria

- [ ] GameSession privately owns Simulation and SimulationScheduler in a library without graphics dependencies.
- [ ] main.cpp advances and reads the tick through one GameSession instance.
- [ ] Debug build succeeds and all existing 17 tests pass.
- [ ] Desktop tick rate and pause/resume behavior remain correct after extraction.
