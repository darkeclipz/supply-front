# Select multiple prototype entities

## Description

Exercise app::Selection with several simulation-owned development entities and Shift-click additive selection. Ordinary clicks replace selection or clear on a miss; Shift-click adds a hit and preserves selection on a miss. Use nearest prototype ray hits rather than loop-order selection. Follow docs/ROADMAP.md section 7.2. These remain development fixtures; drag-box selection, obstacle occlusion, ownership/vision eligibility, and movement are later steps.

Tutor checkpoint: active.

- Current step: Interactive verification of multiple-prototype selection; implementation source-reviewed and build/tests verified, desktop checks pending.
- Context: update_and_draw_prototypes() is defined before run(), outside the editor #if, and called inside 3D mode after engine::drawScene. Selection policy is now correctly after the picking loop; minimum distance and smaller stable-ID ties are source-reviewed. Ordinary select(zero) clears; additive add(zero) preserves selection; Selection rejects duplicates. Mouse capture skips selection input and keyboard capture disables Shift addition. All three cubes render from copied session positions; const-reference enumeration and removal of single-ID calls are reviewed.
- Verification: On 2026-10-05, agent reviewed corrected placement, ran cmake --build --preset debug -j 2 successfully (main.cpp compiled and sandbox linked), and ctest --preset debug: all 32 passed. Nearest-hit and input policies are source-reviewed; interactive checks have not been reported. git diff --check reports trailing whitespace at src/main.cpp:233 and :259.
- Next action: Programmer removes trailing spaces on the helper signature and whitespace-only line before additive, checks git diff --check, launches ./build/debug/bin/sandbox, and reports three cubes, ordinary replacement/miss clearing, Shift multiple outlines/no duplicates/miss preservation, inspector capture, nearest hit under overlapping camera angles, and pause behavior. Close only after desktop acceptance evidence.
- Blocker: None.

## Acceptance criteria

- [x] GameSession seeds three distinct stable prototype IDs at the documented initial positions and exposes read-only enumeration with simulation mutation private.
- [x] Desktop renders all three from session-reported copied positions; the single-prototype accessor is removed after migration.
- [x] Picking selects the nearest intersected prototype independently of enumeration order.
- [x] Ordinary click replaces selection or clears on a miss; Shift-click adds without duplicates and preserves selection on a miss; inspector capture preserves selection.
- [ ] Programmer confirms multiple simultaneous outlines and ordinary/Shift-click behavior under varied camera angles and pause.
- [x] Debug build and all tests pass.
