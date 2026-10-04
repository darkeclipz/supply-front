# Build a minimal graybox map

## Description

Replace the rotating-cube demo view with a small flat gameplay ground and stationary graybox obstacles, establishing X/Z world coordinates before ground picking, selectable units, and right-click movement. This is the first visual prototype, not the full two-kilometer reference map. Follow docs/SYSTEM-DESIGN.md and docs/gdd/Supply_Front_Game_Design_Document.md; expand to two useful corridors after movement works.

- Final result: Ground, stationary visual obstacles, and camera inspection verified; minimal graybox map complete.
- Context: src/main.cpp::OrbitCamera targets the origin with pitch 0.9 radians and distance 32; run draws a 20x20 plane at y=0 and red X/blue Z axes at y=0.01. Ground extent is X/Z -10 to +10. assets/scenes/demo.json now contains obstacle-west (position [-4, 1, -3], scale [3, 2, 2]) and obstacle-east (position [4, 0.5, 3], scale [2, 1, 4]); both use models/cube.glb and zero rotation, without spin_degrees_per_second. The unit cube spans -0.5 to +0.5, so these Y positions place the bottoms at y=0. sceneFromJson adds no Spin component without the field; fixedUpdate only rotates entities with Spin. Keep obstacles as scene entities for inspector transforms and snapshots. These are rendering placeholders; collision/navigation and two useful corridors follow later.
- Verification: On 2026-10-04, agent reviewed the scene and source changes and ran cmake --build --preset debug -j 2, ctest --preset debug, cmp assets/scenes/demo.json build/debug/bin/assets/scenes/demo.json, and git diff --check successfully: all 29 tests passed and the staged scene matches the edited asset. Programmer then confirmed the requested desktop checks with "Check, that still works": blocks rest on the ground and remain stationary while ticks advance, finite ground/axes are readable, and orbit/zoom work. Desktop evidence is programmer-reported.
- Next action: Continue todo/open/ground-picking.md to map the cursor to a bounded ground point before selection/movement.

## Acceptance criteria

- [x] Desktop shows bounded flat ground at y=0 with clear X/Z world coordinates and a camera framing the playable area.
- [x] Stationary graybox obstacles replace rotating demo cubes in the map view; their role as visual placeholders is clear.
- [x] Programmer confirms the ground and obstacles are readable and the camera can inspect the map.
- [x] Debug build and existing tests pass.
