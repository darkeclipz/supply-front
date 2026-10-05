# Manage selection and select the prototype with a left click

## Description

Add reusable application-owned selection state supporting multiple stable gameplay IDs, then connect left-click picking and outline feedback for the development cube. Follow docs/ROADMAP.md section 7.2 and docs/SYSTEM-DESIGN.md's stable-ID/presentation boundaries. The programmer requested a selection class now rather than a single selected-ID local. Multiple-ID collection behavior is implemented and tested here; drag-box picking, additional units, decorative obstacle occlusion, player eligibility, and commands follow later.

- Final result: Reusable multi-ID Selection state, headless collection tests, and single-prototype left-click/outline integration verified.
- Context: app::Selection stores unique nonzero stable IDs in insertion order with select/add/remove/clear/contains and const enumeration, independently of raylib/simulation mutation. src/main.cpp owns Selection outside the frame loop, clears it on uncaptured left clicks, raycasts the prototype bounding box using the updated camera, selects its stable ID on hit, and draws a yellow outline when contained. Click handling is independent of playing, and inspector capture prevents selection changes. Ground/hover rendering remains intact. Multi-entity picking/additive modifiers follow next; obstacle occlusion, player eligibility, and commands follow later.
- Verification: On 2026-10-04, agent reviewed the integration source, Selection implementation, and two tests (multiple IDs/order, duplicate/zero rejection, absent/repeated removal, replacement, clear, select(zero)). Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully: all 32 tests passed. Programmer reported the integration works and raycast hits correctly under various camera angles. Capture gating, persistent state, click miss clearing, playing-independent input, and conditional outline are also source-reviewed; desktop evidence is programmer-reported.
- Next action: Continue todo/open/multiple-prototype-selection.md to create several entities and exercise additive selection through the existing Selection class.

## Acceptance criteria

- [x] A graphics-free app::Selection stores unique nonzero stable IDs and supports replace-with-one, additive selection, removal, clearing, membership lookup, and read-only enumeration.
- [x] Headless tests verify multiple-ID state transitions, duplicate/zero handling, insertion order, replacement, removal of present/absent IDs, and clearing.
- [x] An uncaptured left click hitting the prototype's rendered bounding box selects its stable gameplay ID; an uncaptured miss clears selection.
- [x] Mouse input captured by the inspector leaves gameplay selection unchanged.
- [x] Selection persists across frames as application-owned stable-ID state and never mutates simulation.
- [x] Selected prototype has a clear outline; programmer confirms select/deselect/UI capture/orbit/zoom/pause behavior.
- [x] Debug build and existing tests pass.
