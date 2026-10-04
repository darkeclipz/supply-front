# Store and read fixed-point gameplay positions

## Description

Give stable gameplay entities simulation-owned X/Z positions using signed 64-bit integer millimeters, following docs/SYSTEM-DESIGN.md section 3 and docs/ROADMAP.md section 3.2. Establish creation-time position storage and read-only lookup before rendering/selecting gameplay units. The scene JSON obstacles remain presentation placeholders. Movement, player ownership, command coordinate conversion, and map-bound validation are later steps.

Tutor checkpoint: active.

- Current step: Lookup and lifecycle/copy-isolation test implemented and verified. Add the missing direct <optional> include in Simulation.hpp before final closure.
- Context: Keep the name Position (programmer explicitly declined Position2). include/sim/Position.hpp now has signed std::int64_t x/z with zero defaults, default equality, and a ground X/Z millimeter comment. One world meter is 1000 millimeters; integer coordinates help reproducibility, while movement still needs consistent rounding. Simulation::create_entity(Position position = {}) stores the component inside its rollback try block. Add <optional> and public [[nodiscard]] std::optional<Position> position(GameEntityId id) const in Simulation.hpp; define it in Simulation.cpp by looking up m_entities_by_id, returning std::nullopt if missing, otherwise returning m_registry.get<Position>(entity) as a copy. Add one test in tests/simulation_tests.cpp verifying distinct negative/positive initial positions, default origin, invalid IDs, copy modification isolation, and disappearance after destruction while another entity retains its position. This is a low-level headless query; session/player-filtered observation follows before gameplay presentation.
- Verification: On 2026-10-04, agent reviewed Simulation::position: const lookup, nullopt for missing IDs, and by-value Position return are correct. The new test verifies negative/distinct/default positions, invalid IDs, copied-value mutation isolation, destruction, and preservation of another entity's position. Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully: all 30 tests passed. Simulation.hpp uses std::optional without directly including <optional>; the successful build currently relies on transitive includes. This requested header dependency remains to correct.
- Next action: Programmer adds #include <optional> among the standard-library includes in include/sim/Simulation.hpp and builds Debug. Verify the direct include, then close this todo and select the next task toward rendering/selecting a simulation-owned prototype entity.
- Blocker: None.

## Acceptance criteria

- [x] Gameplay positions use signed integer millimeters on X/Z independently of raylib.
- [x] Gameplay entity creation stores a requested initial position and defaults to the origin without changing stable-ID allocation.
- [x] Position lookup returns a copy by stable ID and reports no position for missing or destroyed entities without exposing registry mutation.
- [x] Headless tests verify distinct initial positions, default origin, invalid/destroyed lookup, and that modifying a returned copy cannot change simulation state.
- [x] Debug build and existing tests pass.
