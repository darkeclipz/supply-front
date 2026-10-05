# Select multiple prototype entities

## Description

Exercise app::Selection with several simulation-owned development entities and Shift-click additive selection. Ordinary clicks replace selection or clear on a miss; Shift-click adds a hit and preserves selection on a miss. Use nearest prototype ray hits rather than loop-order selection. Follow docs/ROADMAP.md section 7.2. These remain development fixtures; drag-box selection, obstacle occlusion, ownership/vision eligibility, and movement are later steps.

- Final result: Three simulation-owned prototypes render with nearest-hit ordinary and Shift-click selection; desktop behavior verified by the programmer.
- Context: GameSession exposes const-reference ID enumeration and copied positions with simulation mutation private. main.cpp's update_and_draw_prototypes() lives outside the editor conditional and is called inside 3D mode. It searches all candidates before applying selection once, with smaller stable IDs resolving equal-distance hits. Mouse capture preserves selection; keyboard capture disables Shift addition. The single-prototype accessor is removed.
- Verification: On 2026-10-05, agent reviewed source and ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 32 passed. Programmer reported the requested desktop checks work as intended, covering multiple outlines, ordinary/Shift-click behavior, inspector capture, varied camera angles/nearest hit, and pause. Latest git diff --check still reports spaces on blank src/main.cpp:259; this formatting cleanup is outstanding but does not affect the verified acceptance criteria.
- Next action: Remove spaces on src/main.cpp:259. Choose the next small task from drag-box selection (roadmap 7.2.2) or a command-driven motion probe (7.2.3/7.3.2); neither is selected or active yet.

## Acceptance criteria

- [x] GameSession seeds three distinct stable prototype IDs at the documented initial positions and exposes read-only enumeration with simulation mutation private.
- [x] Desktop renders all three from session-reported copied positions; the single-prototype accessor is removed after migration.
- [x] Picking selects the nearest intersected prototype independently of enumeration order.
- [x] Ordinary click replaces selection or clears on a miss; Shift-click adds without duplicates and preserves selection on a miss; inspector capture preserves selection.
- [x] Programmer confirms multiple simultaneous outlines and ordinary/Shift-click behavior under varied camera angles and pause.
- [x] Debug build and all tests pass.
