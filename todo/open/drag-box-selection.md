# Select prototypes with a drag box

## Description

Extend prototype group selection with a screen-space drag rectangle, following docs/ROADMAP.md 7.2.2. Commit selection on release: gestures below a 4-pixel threshold use existing nearest-ray click picking; drags select prototypes whose projected cube centers lie inside the normalized rectangle and are in front of the camera. Ordinary gestures replace selection; Shift gestures add without duplicates and preserve selection on empty results. Capture additive intent at press when keyboard input is not captured. Mouse capture cancels an active gesture without committing selection. Selection remains application-owned stable IDs with no simulation mutation. Decorative obstacle occlusion and gameplay eligibility remain future work.

Tutor checkpoint: active.

- Current step: Fix inverted gesture dispatch in update_and_draw_prototypes(); box helper implemented, dispatch correction proposed.
- Context: apply_box_selection() correctly clears once for ordinary boxes, checks front-camera depth, projects copied centers, and adds matching stable IDs. Dispatch currently uses drag.completed && !drag.dragging for the box branch, incorrectly routing clicks to near-zero boxes and drags to ray picking. Remove ! so box selection runs for completed drags. Remaining else-if(completed) handles clicks. Change click ray to drag.current for gesture-state consistency. Rectangle and capture state remain intact.
- Verification: On 2026-10-05, programmer reported selection stopped working after box integration. Agent source review identified the inverted predicate at src/main.cpp:339. Agent ran cmake --build --preset debug -j 2 successfully (sandbox compiled/linked), and git diff --check passed. No tests rerun for this review; earlier 32/32 result predates box integration. Current click acceptance is unchecked due to regression; new desktop behavior remains unverified.
- Next action: Programmer removes ! before drag.dragging in the box branch, uses drag.current for click ray, rebuilds Debug, and verifies ordinary/Shift click and box behavior, empty boxes, all drag directions, camera angles, capture cancellation and pause. Agent will rerun tests after correction and close only with desktop evidence.
- Blocker: None; concrete source defect identified.

## Acceptance criteria

- [x] A visible normalized screen-space rectangle follows drags in every direction after a small movement threshold and disappears on release or cancellation.
- [ ] Clicks below the threshold retain nearest-hit ordinary/Shift-click behavior and selection is committed once on release.
- [ ] Drag release selects every prototype whose projected cube center lies inside the rectangle, excluding centers behind the camera.
- [ ] Ordinary drag replaces selection and clears on an empty box; Shift-drag adds without duplicates and preserves selection on an empty box, using uncaptured keyboard intent at press.
- [x] Inspector-owned input never starts or commits a gameplay gesture; interrupted gestures cancel without selection changes.
- [ ] Programmer verifies box/click behavior, multiple outlines, all drag directions, varied camera angles, UI capture, and pause.
- [x] Debug build and existing tests pass.
