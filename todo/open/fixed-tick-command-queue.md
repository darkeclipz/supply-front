# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: paused.

- Current step: Equivalence test size/result guards implemented and verified; one target-existence assertion correction remains. Paused while graybox map is active.
- Context: The test now checks outcome sizes before indexing and explicit accepted/missing_entity results. It still uses REQUIRE_FALSE(reference.entity_exists(varied_target)); replace reference with varied on that line. Both fresh simulations assign the same numeric ID, so the current assertion checks the reference twice. GameSession forwarding remains deferred until gameplay needs it.
- Verification: On 2026-10-04, agent reviewed saved test, ran cmake --build --preset debug -j 2 successfully without compiler diagnostics and ctest --preset debug: all 29 tests passed. Outcome comparison and size/result guards verified. Final varied-target absence remains unchecked because of the wrong simulation reference.
- Next action: Programmer changes the varied_target assertion to varied.entity_exists(varied_target), then continues the ground-plane/camera step in todo/open/graybox-map.md. Rerun existing build/tests after edits. Resume GameSession forwarding when movement needs it; do not close this todo yet.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [x] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [x] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [x] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
