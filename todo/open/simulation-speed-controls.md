# Add simulation speed controls

## Description

Add offline 1x, 2x, and 4x gameplay speed controls as planned in docs/SYSTEM-DESIGN.md. Scale the scheduler's elapsed time while keeping each authoritative tick fixed at 100 ms. Keep pause independent and preserve accumulated simulation time when speed changes. Demo animation remains separate.

Tutor checkpoint: active.

- Current step: GameSession speed forwarding verified by source inspection and successful build. Desktop 1x/2x/4x controls proposed, not implemented.
- Context: Scheduler accepts integer speeds 1–16; desktop presets remain 1x/2x/4x. GameSession::advance now accepts int speed = 1 and forwards it to the scheduler while retaining private simulation ownership. Keep demo animation at its existing rate.
- Verification: On 2026-10-03, agent inspected include/app/GameSession.hpp and src/app/GameSession.cpp, ran cmake --build --preset debug -j 2 successfully, and ctest --preset debug: all 21 tests passed. Tests cover Simulation/Scheduler, not GameSession directly. Desktop faster-speed behavior has not been verified.
- Next action: Programmer adds int simulation_speed = 1 beside playing in src/main.cpp::run, passes it by reference to Editor::draw, adds ImGui radio buttons for 1x/2x/4x beside the play checkbox, and forwards it to session.advance. Rebuild, run tests, then launch build/debug/bin/sandbox and check tick deltas over ten seconds (about 100/200/400), pause, speed selection while paused, and resume. Leave engine demo elapsed/step calculations unchanged.
- Blocker: None.

## Acceptance criteria

- [x] Scheduler supports 1x, 2x, and 4x while retaining fixed 10 Hz simulation ticks.
- [x] Headless tests verify speed changes, remainder preservation, pause, invalid speeds, and equivalent elapsed-time partitions at faster speeds.
- [x] GameSession exposes speed control without exposing simulation mutation.
- [ ] Desktop controls select 1x, 2x, and 4x; reported tick rates are approximately 10, 20, and 40 per second and pause/resume still works.
- [ ] Debug build and all tests pass.
