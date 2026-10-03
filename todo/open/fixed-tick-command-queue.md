# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Post-execution tick/sequence validation verified. Add scheduler partition/pause/speed equivalence test; proposed, not implemented.
- Context: Test two fresh simulations with identical target IDs and commands at ticks 2 and 4 targeting the same entity. Reference scheduler gets 450 ms at 1x. Varied scheduler gets five 5 ms frames at 1x, a paused 10 s at 4x (assert no tick/outcome/entity mutation), 25 ms at 1x, three 25 ms frames at 2x, five 10 ms frames at 4x, and 50 ms at 1x. Both accumulate 450 ms of simulation time, reach tick 4 with 50 ms remainder, and produce accepted then missing_entity outcomes. Compare all command fields and results, plus target existence. Advance both another 50 ms at 1x to verify matching remainder and no duplicate outcomes.
- Verification: On 2026-10-03, agent confirmed the corrected accessor call, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 28 tests passed. New validation test proves past/current tick rejection, completed sequence non-reuse, and rejected submissions not consuming sequence 2. Command behavior across scheduler timing changes is not tested yet.
- Next action: Programmer adds one scheduler equivalence test in tests/simulation_tests.cpp, rebuilds Debug, and runs ctest (29 expected). Then forward command submission/read-only outcomes through GameSession and test that boundary before closing.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [x] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
