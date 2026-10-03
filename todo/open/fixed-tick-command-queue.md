# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Submission storage and validation verified. Add due-tick execution and read-only outcome history; proposed, not implemented.
- Context: Submission requires future tick and increasing per-player sequence; rejected submissions do not consume sequences. Add CommandExecution { accepted, missing_entity }, CommandOutcome { Command command; CommandExecution result; }, private vector<CommandOutcome> m_command_outcomes, and const-reference command_outcomes() accessor. tick guards tick overflow, sorts pending commands lexicographically by execute_at/player_id/sequence, finds the due prefix for current_tick + 1, reserves outcome capacity before mutations, destroys each due target and records its result, erases processed commands, then commits the tick. Outcome history is cumulative for this small foundation; retention policy follows later. Development-only destroy fixture has no ownership/visibility checks.
- Verification: On 2026-10-03, agent reviewed source and full submission test, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 25 tests passed. Tick 0, sequence 0, duplicate/lower sequences, independent players, and absence of immediate mutation verified. Execution/order/future retention are not implemented yet.
- Next action: Programmer adds outcome types/accessor/storage and replaces tick with ordered due execution (algorithm/tuple includes). Add one test scheduling two destroys of the same target at ticks 2 and 3; check no execution at tick 1, accepted destruction at tick 2, missing_entity at tick 3, and no repeated outcomes at tick 4. Rebuild Debug and run ctest (26 expected). Then add same-tick ordering, sequence reuse/late-tick tests, scheduler equivalence, and GameSession forwarding.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [ ] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [ ] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
