# Build a minimal graybox map

## Description

Replace the rotating-cube demo view with a small flat gameplay ground and stationary graybox obstacles, establishing X/Z world coordinates before ground picking, selectable units, and right-click movement. This is the first visual prototype, not the full two-kilometer reference map. Follow docs/SYSTEM-DESIGN.md and docs/gdd/Supply_Front_Game_Design_Document.md; expand to two useful corridors after movement works.

- Status: Planned next, after the scheduler equivalence test in todo/open/fixed-tick-command-queue.md is verified. No active tutor checkpoint yet.
- Context: src/main.cpp::run currently draws DrawGrid and the engine demo scene. OrbitCamera targets y=0.7 and starts close to rotating cubes loaded from assets/scenes/demo.json. Existing scene schema supports model transforms and optional Spin. Start with a bounded flat ground at y=0, static cube obstacles, and a camera that shows the map; keep rendering data distinct from authoritative simulation state. Obstacles are visual placeholders until navigation is implemented.
- Verification: Existing source, scene JSON, TEMPLATE.md, and design inspected on 2026-10-03. No map code changed or checks run for this task.
- Next action: After finishing the equivalence test, inspect current map/render/camera code again and guide the programmer through the first visible ground-plane change. Use the running desktop for visual verification and existing build/tests for regressions; add focused tests for gameplay behavior when needed.

## Acceptance criteria

- [ ] Desktop shows bounded flat ground at y=0 with clear X/Z world coordinates and a camera framing the playable area.
- [ ] Stationary graybox obstacles replace rotating demo cubes in the map view; their role as visual placeholders is clear.
- [ ] Programmer confirms the ground and obstacles are readable and the camera can inspect the map.
- [ ] Debug build and existing tests pass.
