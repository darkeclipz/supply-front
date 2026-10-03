# Add simulation speed controls

## Description

Add offline 1x, 2x, and 4x gameplay speed controls as planned in docs/SYSTEM-DESIGN.md. Scale the scheduler's elapsed time while keeping each authoritative tick fixed at 100 ms. Keep pause independent and preserve accumulated simulation time when speed changes. Demo animation remains separate.

- Final result: Scheduler speed scaling, GameSession forwarding, and desktop 1x/2x/4x/8x controls complete. Fixed ticks remain 100 ms; demo animation timing stays separate.
- Context: Scheduler accepts integer speeds 1–16; GameSession privately owns simulation and scheduler and defaults to 1x. Desktop UI exposes 1x/2x/4x plus the programmer's added 8x option.
- Verification: On 2026-10-03, agent inspected source and ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 21 tests passed. Tests cover scheduler speeds, bounds, remainder preservation, pause, and partition equivalence. Programmer reports pause freezes ticks, switching speed works, resuming causes no catch-up, and approximate 10/20/40/80 ticks per second feels consistent with the requested checks. Rates are a qualitative user observation, not an instrumented measurement.
- Next action: Select the next foundation task; stable gameplay entity IDs are a suitable dependency before command targeting and replay checksums.

## Acceptance criteria

- [x] Scheduler supports 1x, 2x, and 4x while retaining fixed 10 Hz simulation ticks.
- [x] Headless tests verify speed changes, remainder preservation, pause, invalid speeds, and equivalent elapsed-time partitions at faster speeds.
- [x] GameSession exposes speed control without exposing simulation mutation.
- [x] Desktop controls select 1x, 2x, and 4x; reported tick rates are approximately 10, 20, and 40 per second and pause/resume still works.
- [x] Debug build and all tests pass.
