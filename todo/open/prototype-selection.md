# Manage selection and select the prototype with a left click

## Description

Add reusable application-owned selection state supporting multiple stable gameplay IDs, then connect left-click picking and outline feedback for the development cube. Follow docs/ROADMAP.md section 7.2 and docs/SYSTEM-DESIGN.md's stable-ID/presentation boundaries. The programmer requested a selection class now rather than a single selected-ID local. Multiple-ID collection behavior is implemented and tested here; drag-box picking, additional units, decorative obstacle occlusion, player eligibility, and commands follow later.

Tutor checkpoint: active.

- Current step: Add dedicated headless Selection tests and CMake target; proposed, not implemented. Selection implementation is complete and source/build reviewed.
- Context: Create include/app/Selection.hpp and src/app/Selection.cpp. Selection owns a private std::vector<sim::GameEntityId> m_entities, preserving insertion order and rejecting zero/duplicate IDs. Public methods: select(id) replaces selection via clear/add; add(id) adds uniquely; remove(id) uses std::erase; clear() noexcept empties it; contains(id) const uses std::find; entities() const noexcept returns a const vector reference. IDs retain the existing default equality. Picking, modifier input, simulation eligibility/death filtering, and drawing stay outside this class. Add Selection.cpp alongside GameSession.cpp to the existing supply_session CMake target, retaining a graphics-free app library. No drag-box/multi-unit UI is required yet; collection tests will establish multi-ID semantics before prototype click integration.
- Verification: On 2026-10-04, agent confirmed clear() noexcept now calls m_entities.clear(), with all other methods and graphics-free supply_session source wiring correct. Agent ran cmake --build --preset debug -j 2 successfully. Programmer previously reported existing tests passing; those tests do not exercise Selection. Agent diff check still reports trailing whitespace on CMakeLists.txt line 38; line 40 is corrected. New collection tests have not been added or run.
- Next action: Programmer creates tests/selection_tests.cpp with the supplied two cases: unique/nonzero/insertion-ordered additive selection plus present/absent/repeated removal; and select replacement, clear, and select(zero) clearing. Add selection_tests target inside SEED_BUILD_TESTS after simulation_tests, link supply_session and Catch2::Catch2WithMain, apply seed_warnings and catch_discover_tests. Trim remaining trailing whitespace on add_library(supply_session STATIC while editing CMake. Build Debug and run ctest --preset debug; expect 32 tests if exactly the two supplied cases are added. Review tests/results before prototype input integration.
- Blocker: None.

## Acceptance criteria

- [ ] A graphics-free app::Selection stores unique nonzero stable IDs and supports replace-with-one, additive selection, removal, clearing, membership lookup, and read-only enumeration.
- [ ] Headless tests verify multiple-ID state transitions, duplicate/zero handling, insertion order, replacement, removal of present/absent IDs, and clearing.
- [ ] An uncaptured left click hitting the prototype's rendered bounding box selects its stable gameplay ID; an uncaptured miss clears selection.
- [ ] Mouse input captured by the inspector leaves gameplay selection unchanged.
- [ ] Selection persists across frames as application-owned stable-ID state and never mutates simulation.
- [ ] Selected prototype has a clear outline; programmer confirms select/deselect/UI capture/orbit/zoom/pause behavior.
- [ ] Debug build and existing tests pass.
