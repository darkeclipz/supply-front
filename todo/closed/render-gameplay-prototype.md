# Render a simulation-owned prototype entity

## Description

Seed one development entity in GameSession and draw its position through a read-only session API, preparing for selection and movement. Follow docs/SYSTEM-DESIGN.md's separation of simulation state and presentation. This is a single-entity development fixture with no player/visibility state, not a complete unit definition or production observation API. Player-filtered observations follow when ownership/vision exists; JSON scene obstacles remain presentation placeholders.

- Final result: Session-owned prototype fixture, read-only presentation access, and desktop rendering verified.
- Context: GameSession creates m_prototype_entity at sim::Position{2000, 1000}, exposes its stable ID, and forwards optional position copies from private m_simulation. src/main.cpp::run now queries this position immediately after engine::drawScene and before the hover-marker block, converting x/z with static_cast<float>/1000.0f and drawing a BLUE 0.8x1x0.8 cube centered at y=0.5. Missing entities produce no cube. Scene obstacles and yellow hover marker remain; rendering does not mutate simulation state.
- Verification: On 2026-10-04, agent reviewed the main.cpp drawing and GameSession fixture/accessors: stable-ID lookup, optional guard, millimeter-to-meter conversion, private ownership, dimensions, height, and 3D placement are correct. Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully; all 30 tests passed. Programmer confirmed "The cube is visible" and then "its clear on the ground and visible", satisfying placement/readability checks. Desktop evidence is programmer-reported; existing tests cover Simulation/Scheduler, not desktop rendering or GameSession directly.
- Next action: Continue todo/open/prototype-selection.md to select the prototype by stable ID and draw selection feedback.

## Acceptance criteria

- [x] GameSession initializes one development entity at X=2000mm/Z=1000mm and exposes its stable ID with simulation mutation private.
- [x] Session position lookup returns an optional copy and preserves missing-entity behavior.
- [x] Desktop draws the prototype from its session-reported position, converting millimeters to rendering meters only at the presentation boundary.
- [x] Programmer confirms the prototype rests on clear ground at X=2m/Z=1m and is visually distinct from obstacles and the hover marker.
- [x] Debug build and existing tests pass.
