# Add stable gameplay entity IDs

## Description

Give simulation entities permanent identities independent of EnTT handles, as required by docs/SYSTEM-DESIGN.md. Implement simulation-owned creation, existence lookup, and destruction with monotonically allocated IDs that are never reused within a simulation. Keep the registry private and leave GameSession mutation APIs and serialization for later command/save tasks.

Tutor checkpoint: active.

- Current step: GameEntityId type verified by source review and successful Debug build. Simulation::create_entity allocation and its headless test proposed, not implemented.
- Context: GameEntityId wraps std::uint64_t, defaults to zero, and has defaulted equality. Simulation includes it and retains a private registry. Next add [[nodiscard]] GameEntityId create_entity(), private std::uint64_t m_last_entity_id = 0, and creation storing the ID as an EnTT component. Check max before increment; increment allocation state only after successful component insertion and destroy the new EnTT entity if insertion throws. Lookup and destruction APIs follow later; no GameSession mutation API in this task.
- Verification: On 2026-10-03, agent reviewed include/sim/GameEntityId.hpp and Simulation.hpp, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 21 tests passed. Allocation behavior is not implemented or tested yet.
- Next action: Programmer adds create_entity declaration and last-ID member in include/sim/Simulation.hpp, defines it in src/sim/Simulation.cpp with limits/stdexcept includes, and adds a test in tests/simulation_tests.cpp asserting IDs 1/2/3 and identical allocation in two fresh simulations. Rebuild Debug and run ctest; expected 22 tests. Then review and add stable-ID lookup/destruction.
- Blocker: None.

## Acceptance criteria

- [x] GameEntityId is a distinct equality-comparable 64-bit value type; zero represents an invalid ID.
- [ ] Simulation creates entities with unique, nonzero, monotonically allocated IDs independent of EnTT handles; exhausted IDs are rejected before wraparound.
- [ ] Existence lookup and destruction use stable IDs without exposing registry mutation; destroyed IDs are not reused and unknown IDs are handled safely.
- [ ] Headless tests verify deterministic allocation, lookup, destruction, and non-reuse.
- [ ] Debug build and all tests pass.
