# Render a simulation-owned prototype entity

## Description

Seed one development entity in GameSession and draw its position through a read-only session API, preparing for selection and movement. Follow docs/SYSTEM-DESIGN.md's separation of simulation state and presentation. This is a single-entity development fixture with no player/visibility state, not a complete unit definition or production observation API. Player-filtered observations follow when ownership/vision exists; JSON scene obstacles remain presentation placeholders.

Tutor checkpoint: active.

- Current step: Prototype visibility confirmed by programmer; ground placement/readability confirmation remains. Session fixture/accessors and rendering are implemented and source/build reviewed.
- Context: GameSession creates m_prototype_entity at sim::Position{2000, 1000}, exposes its stable ID, and forwards optional position copies from private m_simulation. src/main.cpp::run now queries this position immediately after engine::drawScene and before the hover-marker block, converting x/z with static_cast<float>/1000.0f and drawing a BLUE 0.8x1x0.8 cube centered at y=0.5. Missing entities produce no cube. Scene obstacles and yellow hover marker remain; rendering does not mutate simulation state.
- Verification: On 2026-10-04, agent reviewed the actual main.cpp drawing diff: session lookup, optional guard, millimeter-to-meter conversion, dimensions, height, and 3D draw placement match the proposed step. Previous source review verified GameSession fixture/accessors/private ownership. Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully; all 30 tests passed. Programmer then reported "The cube is visible", confirming desktop rendering. Existing tests cover Simulation/Scheduler, not desktop rendering or GameSession directly. Explicit confirmation of the cube resting on clear ground and being distinct from obstacles/marker remains.
- Next action: Programmer launches ./build/debug/bin/sandbox and confirms a stationary blue cube rests on clear ground at X=2m/Z=1m, distinct from obstacles/yellow hover marker, with orbit/zoom working. Close this todo after desktop criteria are confirmed, then prepare single-entity selection.
- Blocker: None.

## Acceptance criteria

- [x] GameSession initializes one development entity at X=2000mm/Z=1000mm and exposes its stable ID with simulation mutation private.
- [x] Session position lookup returns an optional copy and preserves missing-entity behavior.
- [x] Desktop draws the prototype from its session-reported position, converting millimeters to rendering meters only at the presentation boundary.
- [ ] Programmer confirms the prototype rests on clear ground at X=2m/Z=1m and is visually distinct from obstacles and the hover marker.
- [x] Debug build and existing tests pass.
