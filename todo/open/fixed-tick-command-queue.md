# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Post-execution tick/sequence validation test proposed; not present in the saved workspace despite the programmer reporting done.
- Context: tests/simulation_tests.cpp still ends with the verified same-tick ordering test. No test titled "Completed commands cannot reuse their sequence" or equivalent post-execution assertions were found under tests. The next step remains adding that test: execute sequence 1, advance to tick 2, reject tick 1 and tick 2 using fresh sequence 2, reject sequence 1 for future tick 3, then accept and execute sequence 2 at tick 3.
- Verification: On 2026-10-03, agent inspected the saved test file and searched tests for the proposed test, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 27 existing tests passed. The expected 28th test is absent; user may have an unsaved editor buffer. No new validation behavior was verified this turn.
- Next action: Programmer saves/adds the proposed "Completed commands cannot reuse their sequence" test in tests/simulation_tests.cpp, rebuilds Debug, and runs ctest (28 expected). Then verify the saved test before advancing to scheduler equivalence and GameSession forwarding.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [x] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
