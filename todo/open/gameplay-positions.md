# Store and read fixed-point gameplay positions

## Description

Give stable gameplay entities simulation-owned X/Z positions using signed 64-bit integer millimeters, following docs/SYSTEM-DESIGN.md section 3 and docs/ROADMAP.md section 3.2. Establish creation-time position storage and read-only lookup before rendering/selecting gameplay units. The scene JSON obstacles remain presentation placeholders. Movement, player ownership, command coordinate conversion, and map-bound validation are later steps.

Tutor checkpoint: active.

- Current step: Add read-only position lookup and one headless lifecycle/copy-isolation test; proposed, not implemented. Position type and creation-time storage are corrected and source/build reviewed.
- Context: Keep the name Position (programmer explicitly declined Position2). include/sim/Position.hpp now has signed std::int64_t x/z with zero defaults, default equality, and a ground X/Z millimeter comment. One world meter is 1000 millimeters; integer coordinates help reproducibility, while movement still needs consistent rounding. Simulation::create_entity(Position position = {}) stores the component inside its rollback try block. Add <optional> and public [[nodiscard]] std::optional<Position> position(GameEntityId id) const in Simulation.hpp; define it in Simulation.cpp by looking up m_entities_by_id, returning std::nullopt if missing, otherwise returning m_registry.get<Position>(entity) as a copy. Add one test in tests/simulation_tests.cpp verifying distinct negative/positive initial positions, default origin, invalid IDs, copy modification isolation, and disappearance after destruction while another entity retains its position. This is a low-level headless query; session/player-filtered observation follows before gameplay presentation.
- Verification: On 2026-10-04, agent inspected the corrected signed x/z fields and unchanged creation rollback. Agent ran cmake --build --preset debug -j 2 (simulation/session/tests/sandbox rebuilt), ctest --preset debug, and git diff --check successfully: all 29 tests passed. Existing tests confirm stable-ID behavior but do not yet verify stored coordinates. No checks have run for the proposed query/test.
- Next action: Programmer adds the optional-returning lookup and supplied position lifecycle test, runs cmake --build --preset debug -j 2 and ctest --preset debug (expect 30 test cases after adding the one new TEST_CASE), then returns for review.
- Blocker: None.

## Acceptance criteria

- [x] Gameplay positions use signed integer millimeters on X/Z independently of raylib.
- [ ] Gameplay entity creation stores a requested initial position and defaults to the origin without changing stable-ID allocation.
- [ ] Position lookup returns a copy by stable ID and reports no position for missing or destroyed entities without exposing registry mutation.
- [ ] Headless tests verify distinct initial positions, default origin, invalid/destroyed lookup, and that modifying a returned copy cannot change simulation state.
- [x] Debug build and existing tests pass.
