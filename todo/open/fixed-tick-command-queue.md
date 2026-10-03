# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Interleaved ordering test corrected and verified. Add post-execution tick/sequence validation test; proposed, not implemented.
- Context: Outcomes are sorted by tick/player/sequence; future commands retained. Corrected final assertion uses outcomes[4].result and missing_entity loop starts at 1. Next test must advance to tick 2, reject both past tick 1 and current tick 2 without consuming sequence 2, reject completed sequence 1 when resubmitted for future tick 3, then accept fresh sequence 2 for tick 3 and verify its execution. No production changes expected for this step.
- Verification: On 2026-10-03, agent inspected corrections, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 27 tests passed. Ordering test confirms tick-1 identity order (0,2)/(0,3)/(1,1)/(1,2), accepted first execution, later missing targets, and retained tick-3 command despite earlier submission. Post-execution sequence reuse and nonzero past/current tick rejection are not specifically tested yet.
- Next action: Programmer adds one post-execution validation test to tests/simulation_tests.cpp, rebuilds Debug, and runs ctest (28 expected). Then add scheduler partition/pause/speed equivalence tests and GameSession forwarding.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [x] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
