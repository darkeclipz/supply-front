# Add simulation speed controls

## Description

Add offline 1x, 2x, and 4x gameplay speed controls as planned in docs/SYSTEM-DESIGN.md. Scale the scheduler's elapsed time while keeping each authoritative tick fixed at 100 ms. Keep pause independent and preserve accumulated simulation time when speed changes. Demo animation remains separate.

Tutor checkpoint: active.

- Current step: Scheduler scaling and 2x remainder test verified. Add tests for changing speed across pause and invalid speed bounds; proposed, not implemented.
- Context: Programmer implemented integer speeds 1 through 16 inclusive rather than the suggested discrete 1/2/4 set. Retain this range and test its bounds; desktop preset choices remain 1x/2x/4x. Validation precedes the pause return. Existing callers retain 1x behavior. GameSession and UI wiring follow after scheduler verification.
- Verification: Agent reviewed current source on 2026-10-03; cmake --build --preset debug -j 2 succeeded (asset staging only), and ctest --preset debug passed all 18 tests, including the new 2x partial-tick test. Speed changes, faster pause, invalid bounds, 4x, and faster partition equivalence still need test evidence.
- Next action: The programmer will add the two proposed tests in tests/simulation_tests.cpp and run the Debug build and tests. Expected total: 20 tests. Faster partition equivalence follows next.
- Blocker: None.

## Acceptance criteria

- [ ] Scheduler supports 1x, 2x, and 4x while retaining fixed 10 Hz simulation ticks.
- [ ] Headless tests verify speed changes, remainder preservation, pause, invalid speeds, and equivalent elapsed-time partitions at faster speeds.
- [ ] GameSession exposes speed control without exposing simulation mutation.
- [ ] Desktop controls select 1x, 2x, and 4x; reported tick rates are approximately 10, 20, and 40 per second and pause/resume still works.
- [ ] Debug build and all tests pass.
