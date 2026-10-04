# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

- Final result: Fixed-tick command queue and GameSession submission/read-only outcome forwarding complete.
- Context: GameSession exposes [[nodiscard]] submit_command(sim::Command) and command_outcomes() const noexcept, directly forwarding to private m_simulation. The result is a const vector reference; callers must not retain references to vector elements across ticks. The destruction payload remains a development fixture and is not connected to mouse input. Ownership/visibility validation and real gameplay payloads follow when that state exists.
- Verification: On 2026-10-04, agent source-reviewed both GameSession forwarding declarations/definitions and direct includes, confirming private simulation/scheduler ownership and no mutable registry/result exposure. Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully; all 29 tests passed. Existing Simulation/Scheduler tests verify timing, ordering, sequence validation, future retention, missing targets, and frame/pause/speed equivalence, including accepted/missing_entity outcomes. GameSession forwarding is source-reviewed and build-verified; no dedicated session test was added.
- Next action: Continue todo/open/gameplay-positions.md to establish simulation-owned positions before rendering/selecting gameplay entities.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [x] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [x] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [x] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [x] Debug build and all tests pass.
