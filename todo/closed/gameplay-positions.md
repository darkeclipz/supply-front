# Store and read fixed-point gameplay positions

## Description

Give stable gameplay entities simulation-owned X/Z positions using signed 64-bit integer millimeters, following docs/SYSTEM-DESIGN.md section 3 and docs/ROADMAP.md section 3.2. Establish creation-time position storage and read-only lookup before rendering/selecting gameplay units. The scene JSON obstacles remain presentation placeholders. Movement, player ownership, command coordinate conversion, and map-bound validation are later steps.

- Final result: Signed fixed-point gameplay positions, creation-time storage, and read-only stable-ID lookup complete.
- Context: Keep the name Position (programmer declined Position2); fields are signed std::int64_t x/z, with zero defaults, default equality, and documented millimeter units. One world meter is 1000 millimeters. Simulation::create_entity(Position position = {}) stores the component inside its rollback try block. Simulation::position(GameEntityId) const returns an optional copy or nullopt for missing IDs. Simulation.hpp directly includes <optional>. No raylib types enter simulation; movement still needs deterministic rounding/remainder rules.
- Verification: On 2026-10-04, agent reviewed creation/lookup implementation and the lifecycle test covering negative/distinct/default positions, invalid IDs, copied-value mutation isolation, destruction, and another entity's preserved position. Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully: all 30 tests passed. After the programmer added the missing <optional> include, agent verified it directly and reran the Debug build and diff check successfully. Tests were not repeated for that include-only correction.
- Next action: Continue todo/open/render-gameplay-prototype.md to seed and render one simulation-owned development entity through GameSession.

## Acceptance criteria

- [x] Gameplay positions use signed integer millimeters on X/Z independently of raylib.
- [x] Gameplay entity creation stores a requested initial position and defaults to the origin without changing stable-ID allocation.
- [x] Position lookup returns a copy by stable ID and reports no position for missing or destroyed entities without exposing registry mutation.
- [x] Headless tests verify distinct initial positions, default origin, invalid/destroyed lookup, and that modifying a returned copy cannot change simulation state.
- [x] Debug build and existing tests pass.
