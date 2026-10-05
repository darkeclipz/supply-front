# Clean up the desktop entry point

## Description

Refactor src/main.cpp into focused desktop modules while supporting switchable fixed isometric and orbit cameras while preserving selection, UI capture, rendering order, command-line options, logging, pause/speed, smoke-test behavior and resource lifetimes. Keep raylib/ImGui dependencies on desktop targets; supply_session and supply_sim must remain graphics-free. Work incrementally: add switchable camera views and extract the camera module, then extract selection interaction/presentation, inspector, and startup/runtime concerns until main.cpp contains startup/error handling rather than all desktop implementation. The programmer makes implementation changes under tutoring.

Tutor checkpoint: active.

- Current step: Extract camera state and orbit update into include/desktop/CameraRig.hpp and src/desktop/CameraRig.cpp; proposed, not implemented.
- Context: Camera toggle implemented correctly in src/main.cpp. Keep isometric_view, inspector/F6 switching, previous-mode snapshot and gesture cancellation in run(). desktop::CameraRig owns both Camera3D values and private m_yaw/m_pitch/m_distance. Expose update(bool isometric_view, bool mouse_captured) and native(bool isometric_view) const noexcept returning const Camera3D&. update returns immediately for isometric mode; orbit math and constants remain unchanged. Compile source directly into sandbox, keeping supply_session/supply_sim graphics-free.
- Verification: On 2026-10-05, agent reviewed diff: shared active_camera used for ground picking, rendering and prototype selection; switching cancels gestures; F6 respects keyboard capture and remains outside editor guard. Agent ran cmake --build --preset debug -j 2 successfully, ctest --preset debug passed all 32 tests, and git diff --check passed. Programmer reports desktop toggle working; individual edge-case checks were not separately reported. Release not yet checked.
- Next action: Programmer adds CameraRig header/source, moves orbit math, removes inline OrbitCamera and local isometric_camera, replaces camera update/access calls, and adds source to sandbox in CMakeLists.txt. Build Debug/run tests; manually confirm both views, restored orbit position, picking and drag cancellation still work. Keep <cmath> in main.cpp for std::fmod.
- Blocker: None.

## Acceptance criteria

- [ ] A dedicated desktop camera module supports fixed orthographic isometric and preserved orbit views, inspector/hotkey switching, consistent rendering/picking, and cancellation of in-progress gestures on mode changes.
- [ ] Gesture state, prototype picking and drawing are extracted from main.cpp with click/box/Shift/capture behavior preserved.
- [ ] Inspector implementation is extracted and builds with the editor enabled and disabled.
- [ ] Startup/runtime concerns are organized so main.cpp is a concise entry point; CLI, logging and smoke-test behavior are preserved.
- [ ] Desktop module extraction preserves window/assets/editor resource lifetimes and rendering/input order; graphics dependencies do not enter supply_session or supply_sim.
- [ ] Debug and editor-free Release builds pass, existing tests pass, and git diff --check is clean.
- [ ] Programmer confirms camera switching/orbit/zoom/isometric framing, selection, inspector, pause/speed and scene reload/save behavior after refactoring.
