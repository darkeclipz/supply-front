# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: paused.

- Current step: Scheduler equivalence test fully corrected and verified. Paused while ground picking is active; GameSession forwarding remains pending.
- Context: Test correctly checks each simulation's target absence, guards outcome indexing with sizes, compares every command field/result, explicitly expects accepted then missing_entity, and confirms matching tick/remainder with no repeated outcomes after another advance. Development destruction fixture remains a headless harness action.
- Verification: On 2026-10-04, agent confirmed REQUIRE_FALSE(varied.entity_exists(varied_target)), ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 29 tests passed. Command timing, ordering, validation, and scheduler partition/pause/speed equivalence now have test evidence.
- Next action: Continue todo/open/ground-picking.md after the verified graybox map. Resume GameSession submission/read-only outcome forwarding when movement needs application access; do not close this todo until forwarding is complete.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [x] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [x] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [x] Debug build and all tests pass.
