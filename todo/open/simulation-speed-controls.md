# Add simulation speed controls

## Description

Add offline 1x, 2x, and 4x gameplay speed controls as planned in docs/SYSTEM-DESIGN.md. Scale the scheduler's elapsed time while keeping each authoritative tick fixed at 100 ms. Keep pause independent and preserve accumulated simulation time when speed changes. Demo animation remains separate.

Tutor checkpoint: active.

- Current step: Scheduler tests and warning cleanup verified. Forward speed through GameSession::advance; proposed, not implemented.
- Context: Programmer implemented integer speeds 1 through 16 inclusive rather than the suggested discrete 1/2/4 set. Retain this range and test its bounds; desktop preset choices remain 1x/2x/4x. Validation precedes the pause return. Existing callers retain 1x behavior. GameSession and UI wiring follow after scheduler verification.
- Verification: On 2026-10-03, agent ran the Debug build successfully and ctest --preset debug: all 21 tests passed. Prior checkpoint records syntax-only compilation of tests/simulation_tests.cpp with no diagnostics. Latest resumption confirms correct [[nodiscard]], the completed extraction todo, and the active speed-control task; Debug build and all 21 tests pass again. GameSession::advance still accepts only elapsed and paused, so speed forwarding remains unimplemented.
- Next action: The programmer will append int speed = 1 to GameSession::advance in include/app/GameSession.hpp, append int speed to its definition in src/app/GameSession.cpp, and forward speed as the fourth scheduler.advance argument. Rebuild Debug and run all 21 tests; UI wiring follows next. Existing tests exercise the scheduler, not GameSession directly.
- Blocker: None. This resumption independently confirmed a clean working tree before checks, a successful Debug build, and all 21 tests passing; the next action above still matches the current source.

## Acceptance criteria

- [x] Scheduler supports 1x, 2x, and 4x while retaining fixed 10 Hz simulation ticks.
- [x] Headless tests verify speed changes, remainder preservation, pause, invalid speeds, and equivalent elapsed-time partitions at faster speeds.
- [ ] GameSession exposes speed control without exposing simulation mutation.
- [ ] Desktop controls select 1x, 2x, and 4x; reported tick rates are approximately 10, 20, and 40 per second and pause/resume still works.
- [ ] Debug build and all tests pass.
