# Build a minimal graybox map

## Description

Replace the rotating-cube demo view with a small flat gameplay ground and stationary graybox obstacles, establishing X/Z world coordinates before ground picking, selectable units, and right-click movement. This is the first visual prototype, not the full two-kilometer reference map. Follow docs/SYSTEM-DESIGN.md and docs/gdd/Supply_Front_Game_Design_Document.md; expand to two useful corridors after movement works.

Tutor checkpoint: active.

- Current step: Add bounded ground and frame it with the camera; proposed, not implemented.
- Context: src/main.cpp::run currently draws DrawGrid(20, 1.0f) and the demo scene. Replace the grid call with DrawPlane centered at origin, size 20x20, and two colored DrawLine3D axes at y=0.01 (X red, Z blue). OrbitCamera target changes to origin, pitch to 0.9, distance to 32. Native camera position is recalculated by update before rendering. Keep demo cubes visible until the following static-obstacle step. Ground extent is X/Z -10 to +10. Rendering-only prototype; gameplay map bounds/navigation follow later.
- Verification: On 2026-10-03, source inspected and existing Debug build/all 29 tests passed before map changes. Desktop visual verification will come from the programmer. On 2026-10-04 the equivalence test size/result guards passed review and all 29 tests passed again; only its varied_target assertion still uses reference instead of varied. Correct that one line alongside this step.
- Next action: Programmer adjusts OrbitCamera target/pitch/distance and replaces the grid draw with ground/axes in src/main.cpp, makes the queued test assertion corrections, builds Debug, runs existing tests, and launches ./build/debug/bin/sandbox. Verify visible finite ground, colored axes, and a useful initial view; then replace rotating cubes with stationary obstacles.
- Blocker: None.

## Acceptance criteria

- [ ] Desktop shows bounded flat ground at y=0 with clear X/Z world coordinates and a camera framing the playable area.
- [ ] Stationary graybox obstacles replace rotating demo cubes in the map view; their role as visual placeholders is clear.
- [ ] Programmer confirms the ground and obstacles are readable and the camera can inspect the map.
- [ ] Debug build and existing tests pass.
