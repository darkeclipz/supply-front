# Store and read fixed-point gameplay positions

## Description

Give stable gameplay entities simulation-owned X/Z positions using signed 64-bit integer millimeters, following docs/SYSTEM-DESIGN.md section 3 and docs/ROADMAP.md section 3.2. Establish creation-time position storage and read-only lookup before rendering/selecting gameplay units. The scene JSON obstacles remain presentation placeholders. Movement, player ownership, command coordinate conversion, and map-bound validation are later steps.

Tutor checkpoint: active.

- Current step: Creation-time Position storage implemented and source-reviewed; Position field types/axis names need correction before read-only lookup.
- Context: Create include/sim/Position.hpp containing Position with std::int64_t x/z defaulting to zero and default equality. Document on the struct that both coordinates are in millimeters: one world meter is 1000 millimeters. Programmer disliked the x_mm naming; use x/z with the unit contract stated once. Integer coordinates give fixed precision and exact representable addition; deterministic movement still needs consistent rounding/remainder rules. Convert to floating-point meters at the presentation boundary later. Include Position.hpp directly in include/sim/Simulation.hpp and change create_entity to create_entity(Position position = {}). Update src/sim/Simulation.cpp's definition to take Position position (without repeating the default) and emplace<Position>(entity, position) inside the existing try block after GameEntityId and before m_entities_by_id insertion. Existing no-argument callers retain origin positions and stable-ID behavior. Preserve rollback if component allocation fails. No raylib types enter simulation.
- Verification: On 2026-10-04, agent reviewed the Position header and Simulation changes. create_entity(Position position = {}) and emplace<Position> inside the rollback try block are correctly implemented. Position currently has std::uint64_t x/y, rather than signed x/z; negative ground coordinates cannot be represented correctly and the second axis disagrees with the X/Z ground contract. Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully: all 29 tests passed, but existing tests do not check position coordinates or catch these issues.
- Next action: Programmer changes Position fields to std::int64_t x/z (zero defaults, millimeter comment, and default equality retained), builds Debug, and runs existing tests. Review the correction before adding a by-value optional position lookup and tests for negative/distinct/default positions, missing IDs, destruction, and copy isolation.
- Blocker: None.

## Acceptance criteria

- [ ] Gameplay positions use signed integer millimeters on X/Z independently of raylib.
- [ ] Gameplay entity creation stores a requested initial position and defaults to the origin without changing stable-ID allocation.
- [ ] Position lookup returns a copy by stable ID and reports no position for missing or destroyed entities without exposing registry mutation.
- [ ] Headless tests verify distinct initial positions, default origin, invalid/destroyed lookup, and that modifying a returned copy cannot change simulation state.
- [x] Debug build and existing tests pass.
