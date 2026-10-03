# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Same-tick ordering test implemented with two transcription errors; correction proposed, not implemented.
- Context: In tests/simulation_tests.cpp's final test, outcomes[4].command.result is invalid: result belongs to CommandOutcome, so use outcomes[4].result. The missing_entity loop starts at i = 0 but outcome 0 is accepted; start at i = 1. Interleaved submissions and other ordering assertions match the proposed test. Queue implementation remains unchanged.
- Verification: On 2026-10-03, agent ran cmake --build --preset debug -j 2; build failed at tests/simulation_tests.cpp:349 because Command has no result member. Compiler emitted error plus Catch2 macro-expansion notes; no separate compiler warnings appeared. Agent found the incorrect loop bound by source inspection. Tests were not run after the failed build; previous 26-test pass predates this test.
- Next action: Programmer removes .command from the final result assertion and changes the missing_entity loop's initial index to 1, rebuilds Debug, and runs ctest (27 expected). Then verify late submissions and post-execution sequence reuse, scheduler equivalence, and GameSession forwarding.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [ ] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
