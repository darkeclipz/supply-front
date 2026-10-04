# Pick bounded ground coordinates from the cursor

## Description

Map the mouse cursor to a point on the graybox ground and display a hover marker, preparing for selection and movement input. Follow docs/SYSTEM-DESIGN.md's application-owned screen/world picking and docs/ROADMAP.md section 7.1. Authoritative coordinate conversion and gameplay commands come later; this step only uses presentation coordinates.

- Final result: Bounded ground picking and hover marker implemented and verified.
- Context: src/main.cpp::run declares RayCollision ground_hit{} each frame immediately after camera.update(mouse_captured). When !mouse_captured, it casts GetScreenToWorldRay(GetMousePosition(), camera.native) and calls GetRayCollisionQuad with corners [-10,0,-10], [-10,0,10], [10,0,10], [10,0,-10]. After engine::drawScene and before EndMode3D, it draws a yellow sphere of radius 0.12 centered at [ground_hit.point.x,0.12,ground_hit.point.z] only when ground_hit.hit. Frame-local reset and mouse capture gating avoid stale markers. Current input locals use snake_case: mouse_captured and keyboard_captured. Ground-only picking ignores obstacle surfaces; obstacles can occlude the marker.
- Verification: On 2026-10-04, agent inspected current source: both snippets are correctly placed and use the updated camera, matching finite ground corners, capture gating, and conditional marker draw. Installed raylib quad implementation rejects parallel/behind/outside intersections. Agent ran cmake --build --preset debug -j 2, ctest --preset debug, and git diff --check successfully; all 29 tests passed. Programmer explicitly confirmed all visual checks work as expected: tracking on exposed ground, disappearance outside ground and over inspector, and tracking after orbit/zoom. Desktop evidence is programmer-reported; existing tests do not exercise desktop picking.
- Next action: Resume todo/open/fixed-tick-command-queue.md to finish the GameSession command interface before gameplay input is connected.

## Acceptance criteria

- [x] Cursor ray intersection is limited to the 20x20 ground at y=0 and uses the updated camera.
- [x] A visible hover marker follows valid ground hits and disappears when no hit exists or the inspector captures the mouse.
- [x] Programmer confirms marker behavior on exposed ground, outside the ground, over the inspector, and after orbit/zoom.
- [x] Debug build and existing tests pass.
