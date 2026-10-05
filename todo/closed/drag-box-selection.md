# Select prototypes with a drag box

## Description

Extend prototype group selection with a screen-space drag rectangle, following docs/ROADMAP.md 7.2.2. Commit selection on release: gestures below a 4-pixel threshold use existing nearest-ray click picking; drags select prototypes whose projected cube centers lie inside the normalized rectangle and are in front of the camera. Ordinary gestures replace selection; Shift gestures add without duplicates and preserve selection on empty results. Capture additive intent at press when keyboard input is not captured. Mouse capture cancels an active gesture without committing selection. Selection remains application-owned stable IDs with no simulation mutation. Decorative obstacle occlusion and gameplay eligibility remain future work.

- Final result: Drag-box selection and release-time nearest-hit click selection are verified. All acceptance criteria are complete.
- Context: SelectionDrag tracks a latched 4-pixel threshold, normalized rectangle, one-frame release completion and press-time Shift intent. Mouse capture cancels without committing selection. update_and_draw_prototypes() dispatches completed drags to apply_box_selection() and clicks to nearest-ray picking. Box selection uses projected cube centers, rejects centers behind the camera, clears once for ordinary gestures and adds unique stable IDs. Selection remains application-owned; simulation mutation stays private. Click ray uses GetMousePosition(), equivalent to drag.current in the current frame order.
- Verification: On 2026-10-05, agent source-reviewed corrected implementation, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug with all 32 tests passing. Latest git diff --check also passed. Programmer explicitly reported testing all requested desktop behaviors successfully: ordinary/Shift clicks and boxes, empty box clearing/preservation, multiple outlines/no duplicates, all drag directions, varied camera angles, inspector cancellation, and pause. Desktop evidence is programmer-reported; existing headless tests cover selection state rather than raylib gesture/projection integration.
- Next action: Choose the next task. Command-driven movement is a suggested continuation of roadmap 7.2.3/7.3.2; it has not been selected or started.

## Acceptance criteria

- [x] A visible normalized screen-space rectangle follows drags in every direction after a small movement threshold and disappears on release or cancellation.
- [x] Clicks below the threshold retain nearest-hit ordinary/Shift-click behavior and selection is committed once on release.
- [x] Drag release selects every prototype whose projected cube center lies inside the rectangle, excluding centers behind the camera.
- [x] Ordinary drag replaces selection and clears on an empty box; Shift-drag adds without duplicates and preserves selection on an empty box, using uncaptured keyboard intent at press.
- [x] Inspector-owned input never starts or commits a gameplay gesture; interrupted gestures cancel without selection changes.
- [x] Programmer verifies box/click behavior, multiple outlines, all drag directions, varied camera angles, UI capture, and pause.
- [x] Debug build and existing tests pass.
