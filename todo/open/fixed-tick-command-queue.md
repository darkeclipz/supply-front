# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Add GameSession command submission and read-only outcome forwarding; proposed, not implemented. Graybox map and ground picking are now closed and verified.
- Context: include/app/GameSession.hpp currently exposes only advance and current_tick. Add public [[nodiscard]] sim::CommandSubmission submit_command(sim::Command command) and [[nodiscard]] const std::vector<sim::CommandOutcome>& command_outcomes() const noexcept, with direct sim/Command.hpp and vector includes. Define both in src/app/GameSession.cpp as forwarding calls to m_simulation. Keep simulation/scheduler private and retain snake_case methods/m_ members. The const-reference result avoids copying and prevents mutation through this API; do not retain references to vector elements across ticks. This prepares the session boundary for future gameplay commands; the current destruction payload remains a development fixture and is not connected to mouse input.
- Verification: On 2026-10-04, agent inspected GameSession and Simulation APIs and confirmed forwarding is still missing. Existing command tests verify timing, ordering, validation, and scheduler equivalence, including guarded outcome comparison and accepted/missing_entity results. Most recent cmake --build --preset debug -j 2 and ctest --preset debug passed all 29 tests during ground-picking review. No checks have run for the proposed forwarding additions; existing tests exercise Simulation/Scheduler rather than GameSession directly.
- Next action: Programmer adds the two declarations/includes and forwarding definitions, runs cmake --build --preset debug -j 2 and ctest --preset debug, then returns for source review. Close this todo only once forwarding and build/test checks are verified.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [x] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [x] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [x] Debug build and all tests pass.
