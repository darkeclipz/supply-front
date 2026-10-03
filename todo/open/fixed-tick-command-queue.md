# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Post-execution validation test now saved; fix its final accessor call before verification.
- Context: The new test matches the intended past/current tick and reused-sequence checks. Final assertion at tests/simulation_tests.cpp:392 uses simulation.command_outcomes[1] instead of simulation.command_outcomes()[1]. command_outcomes is a method; invoke it before indexing the returned vector. No production-code changes needed.
- Verification: On 2026-10-03, agent confirmed the saved test and ran cmake --build --preset debug -j 2. Compilation failed at line 392 with invalid array subscript on an unresolved function type. Tests were not run after build failure. Previous 27-test pass predates this new test.
- Next action: Programmer inserts () after command_outcomes in the final assertion, saves, rebuilds Debug, and runs ctest (28 expected). Then review validation behavior and continue with scheduler equivalence and GameSession forwarding.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [x] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
