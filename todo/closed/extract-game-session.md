# Extract game session ownership

## Description

Introduce the app/GameSession boundary from docs/EXAMPLE-DESIGN.md. GameSession owns Simulation and SimulationScheduler; main.cpp retains the window, input, frame-time conversion, and rendering. Preserve the existing include/src split, snake_case methods, and m_ member prefixes. The temporary engine demo animation remains separate.

- Final result: GameSession extraction and [[nodiscard]] correction verified.
- Context: Expose advance(nanoseconds, bool paused = false) and current_tick() const. Keep simulation mutation private. Current pause choice stays in the existing playing UI variable for this small extraction.
- Verification: On 2026-10-03, source confirms private simulation/scheduler ownership, headless supply_session linking only supply_sim, main.cpp delegation through session, and correct [[nodiscard]] spelling. Agent ran cmake --build --preset debug -j 2 successfully (asset staging only) and ctest --preset debug: all 17 tests passed. The tests exercise Simulation/Scheduler, not GameSession directly. Desktop behavior after extraction has not been explicitly reported.
- Desktop verification: On 2026-10-03, the programmer reported the requested tick-rate, pause, and resume checks work as expected.
- Next action: Continue with todo/open/simulation-speed-controls.md.

## Acceptance criteria

- [x] GameSession privately owns Simulation and SimulationScheduler in a library without graphics dependencies.
- [x] main.cpp advances and reads the tick through one GameSession instance.
- [x] Debug build succeeds and all existing 17 tests pass.
- [x] Desktop tick rate and pause/resume behavior remain correct after extraction.
