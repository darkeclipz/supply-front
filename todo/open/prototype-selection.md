# Manage selection and select the prototype with a left click

## Description

Add reusable application-owned selection state supporting multiple stable gameplay IDs, then connect left-click picking and outline feedback for the development cube. Follow docs/ROADMAP.md section 7.2 and docs/SYSTEM-DESIGN.md's stable-ID/presentation boundaries. The programmer requested a selection class now rather than a single selected-ID local. Multiple-ID collection behavior is implemented and tested here; drag-box picking, additional units, decorative obstacle occlusion, player eligibility, and commands follow later.

Tutor checkpoint: active.

- Current step: Connect prototype left-click picking and outline feedback to app::Selection; proposed, not implemented. Selection collection and headless tests are verified.
- Context: app::Selection stores unique nonzero stable IDs in insertion order, with select/add/remove/clear/contains and const enumeration; it has no raylib/simulation mutation dependency. Selection.cpp is compiled in supply_session; tests/selection_tests.cpp links that library via its dedicated CMake/Catch2 target. Next include app/Selection.hpp in src/main.cpp and add app::Selection selection after GameSession session, outside the frame loop. After engine::drawScene compute selection_click = !mouse_captured && IsMouseButtonPressed(MOUSE_BUTTON_LEFT); clear selection on such a click. In the existing optional prototype-position block, reuse its center to raycast BoundingBox min {x-0.4,0,z-0.4}, max {x+0.4,1,z+0.4} using the updated camera; select(session.prototype_entity()) on hit. Keep BLUE DrawCube; when selection.contains(prototype ID), draw YELLOW DrawCubeWires(center,0.84,1.04,0.84). Keep the hover marker. This initial integration uses ordinary clicks only; additive modifiers and drag-box input follow later.
- Verification: On 2026-10-04, agent reviewed Selection implementation, the two tests, CMake target/linkage/discovery, and current main.cpp. Tests verify multi-ID insertion/order, duplicate/zero rejection, absent/repeated removal, replacement, explicit clear, and select(zero). Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully: all 32 tests passed and previous CMake whitespace issues are resolved. main.cpp still has no selection instance, click handler, or outline; desktop integration is unverified.
- Next action: Programmer adds the include/persistent Selection instance and supplied click/box/outline block in src/main.cpp, builds Debug, runs existing tests, and launches the sandbox. Confirm clicking the cube selects, clicking elsewhere clears, inspector clicks preserve selection, outline survives orbit/zoom, and selection works while paused. Return for source/desktop review.
- Blocker: None.

## Acceptance criteria

- [x] A graphics-free app::Selection stores unique nonzero stable IDs and supports replace-with-one, additive selection, removal, clearing, membership lookup, and read-only enumeration.
- [x] Headless tests verify multiple-ID state transitions, duplicate/zero handling, insertion order, replacement, removal of present/absent IDs, and clearing.
- [ ] An uncaptured left click hitting the prototype's rendered bounding box selects its stable gameplay ID; an uncaptured miss clears selection.
- [ ] Mouse input captured by the inspector leaves gameplay selection unchanged.
- [ ] Selection persists across frames as application-owned stable-ID state and never mutates simulation.
- [ ] Selected prototype has a clear outline; programmer confirms select/deselect/UI capture/orbit/zoom/pause behavior.
- [x] Debug build and existing tests pass.
