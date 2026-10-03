# Add simulation speed controls

## Description

Add offline 1x, 2x, and 4x gameplay speed controls as planned in docs/SYSTEM-DESIGN.md. Scale the scheduler's elapsed time while keeping each authoritative tick fixed at 100 ms. Keep pause independent and preserve accumulated simulation time when speed changes. Demo animation remains separate.

Tutor checkpoint: active.

- Current step: Add a defaulted integer speed parameter to SimulationScheduler::advance and a focused 2x timing test; proposed, not implemented.
- Context: include/sim/SimulationScheduler.hpp, src/sim/SimulationScheduler.cpp, and tests/simulation_tests.cpp. Append int speed = 1 after bool paused; accept only 1, 2, or 4, validate before the pause return, and accumulate elapsed * speed. Existing callers retain 1x behavior. GameSession and UI wiring follow after scheduler verification.
- Verification: Existing Debug build and all 17 tests passed on 2026-10-03 before this step. No speed-control checks run.
- Next action: The programmer will add the scheduler parameter, validation, elapsed scaling, and the proposed 2x test, then build Debug and run ctest --preset debug.
- Blocker: None.

## Acceptance criteria

- [ ] Scheduler supports 1x, 2x, and 4x while retaining fixed 10 Hz simulation ticks.
- [ ] Headless tests verify speed changes, remainder preservation, pause, invalid speeds, and equivalent elapsed-time partitions at faster speeds.
- [ ] GameSession exposes speed control without exposing simulation mutation.
- [ ] Desktop controls select 1x, 2x, and 4x; reported tick rates are approximately 10, 20, and 40 per second and pause/resume still works.
- [ ] Debug build and all tests pass.
