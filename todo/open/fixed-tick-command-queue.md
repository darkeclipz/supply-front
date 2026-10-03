# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Due-tick execution and outcome recording implemented and timing test verified. Correct the overflow exception throw; proposed, not implemented.
- Context: tick sorts by execute_at/player_id/sequence, executes the due prefix, reserves outcome storage before mutation, records accepted/missing_entity, removes processed commands, and advances the completed tick. Source currently says throw new std::overflow_error in tick's exhaustion guard; change to throw std::overflow_error so it throws an exception object, not a heap-allocated pointer. The pointer escapes catch(const std::exception&) and leaks without manual deletion. Creation already throws by value correctly. Outcome history is cumulative. Same-tick ordering, scheduler equivalence, and GameSession forwarding remain later steps.
- Verification: On 2026-10-03, agent reviewed source and timing test, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 26 tests passed. Test verifies no early execution, retained future command, accepted tick-2 destruction, tick-3 missing target, and no repeated outcomes. Tests do not exercise tick exhaustion; the throw-new defect was found by source review. Comparator ordering is source-reviewed but not yet behavior-tested.
- Next action: Programmer removes new from throw in src/sim/Simulation.cpp::tick and reruns Debug build and ctest (26 expected). Then add a same-tick ordering test with interleaved player submissions, followed by post-execution sequence-reuse/late-tick tests, scheduler equivalence, and GameSession forwarding.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [ ] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
