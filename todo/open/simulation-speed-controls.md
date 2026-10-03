# Add simulation speed controls

## Description

Add offline 1x, 2x, and 4x gameplay speed controls as planned in docs/SYSTEM-DESIGN.md. Scale the scheduler's elapsed time while keeping each authoritative tick fixed at 100 ms. Keep pause independent and preserve accumulated simulation time when speed changes. Demo animation remains separate.

Tutor checkpoint: active.

- Current step: Faster frame-partition equivalence verified; correct signed/unsigned comparisons in its four assertions, proposed, not implemented.
- Context: Programmer implemented integer speeds 1 through 16 inclusive rather than the suggested discrete 1/2/4 set. Retain this range and test its bounds; desktop preset choices remain 1x/2x/4x. Validation precedes the pause return. Existing callers retain 1x behavior. GameSession and UI wiring follow after scheduler verification.
- Verification: On 2026-10-03, agent ran the Debug build successfully and ctest --preset debug: all 21 tests passed, including faster partition equivalence and remainder checks. A syntax-only compilation using the existing compile_commands.json flags reproduced four -Wsign-compare warnings at tests/simulation_tests.cpp:165,166,171,172: unsigned sim::Tick compared with signed int expressions. Catch2 macro notes make these four warnings verbose.
- Next action: The programmer will define const auto tick_speed = static_cast<sim::Tick>(speed) within the final test loop and replace 10 * speed and 11 * speed in its assertions with sim::Tick{10} * tick_speed and sim::Tick{11} * tick_speed, then rebuild Debug and rerun tests. GameSession forwarding follows after warning cleanup.
- Blocker: None.

## Acceptance criteria

- [x] Scheduler supports 1x, 2x, and 4x while retaining fixed 10 Hz simulation ticks.
- [x] Headless tests verify speed changes, remainder preservation, pause, invalid speeds, and equivalent elapsed-time partitions at faster speeds.
- [ ] GameSession exposes speed control without exposing simulation mutation.
- [ ] Desktop controls select 1x, 2x, and 4x; reported tick rates are approximately 10, 20, and 40 per second and pause/resume still works.
- [ ] Debug build and all tests pass.
