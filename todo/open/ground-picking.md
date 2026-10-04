# Pick bounded ground coordinates from the cursor

## Description

Map the mouse cursor to a point on the graybox ground and display a hover marker, preparing for selection and movement input. Follow docs/SYSTEM-DESIGN.md's application-owned screen/world picking and docs/ROADMAP.md section 7.1. Authoritative coordinate conversion and gameplay commands come later; this step only uses presentation coordinates.

Tutor checkpoint: active.

- Current step: Add bounded ground ray picking and a hover marker in src/main.cpp::run; proposed, not implemented.
- Context: Immediately after camera.update(mouseCaptured), declare RayCollision ground_hit{} each frame. When !mouseCaptured, cast GetScreenToWorldRay(GetMousePosition(), camera.native) and call GetRayCollisionQuad with ground corners [-10,0,-10], [-10,0,10], [10,0,10], [10,0,-10]. After engine::drawScene and before EndMode3D, draw a yellow sphere of radius 0.12 centered at [ground_hit.point.x,0.12,ground_hit.point.z] only if ground_hit.hit. Resetting the result each frame prevents stale markers on misses/UI capture. Ground-only picking intentionally ignores obstacle surfaces; the marker can be occluded by obstacles.
- Verification: No checks run for this proposed step. Previous graybox map build/all 29 tests passed and the programmer confirmed desktop checks on 2026-10-04. Agent inspected the installed raylib declarations and quad implementation: it tests two triangles, rejects parallel/behind/outside intersections, and returns hit/point.
- Next action: Programmer adds the two focused picking/marker snippets, builds Debug, runs existing tests, and checks the marker on exposed ground, outside the ground, over the inspector, and while orbiting/zooming.
- Blocker: None.

## Acceptance criteria

- [ ] Cursor ray intersection is limited to the 20x20 ground at y=0 and uses the updated camera.
- [ ] A visible hover marker follows valid ground hits and disappears when no hit exists or the inspector captures the mouse.
- [ ] Programmer confirms marker behavior on exposed ground, outside the ground, over the inspector, and after orbit/zoom.
- [ ] Debug build and existing tests pass.
