# Add stable gameplay entity IDs

## Description

Give simulation entities permanent identities independent of EnTT handles, as required by docs/SYSTEM-DESIGN.md. Implement simulation-owned creation, existence lookup, and destruction with monotonically allocated IDs that are never reused within a simulation. Keep the registry private and leave GameSession mutation APIs and serialization for later command/save tasks.

- Final result: Stable gameplay entity IDs, simulation-owned allocation, existence lookup, destruction, and non-reuse verified. Registry and EnTT handles remain private.
- Context: GameEntityId is a distinct equality-comparable uint64_t wrapper with zero invalid. Simulation allocates from 1, stores IDs as components, and indexes numeric IDs to EnTT handles. Failed component/index insertion cleans up the new entity. Destruction returns false for absent IDs and never decreases m_last_entity_id. IDs are scoped to one simulation; serialization and GameSession command access remain later work.
- Verification: On 2026-10-03, agent reviewed final source and tests, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 24 tests passed. Headless tests verify matching allocation across fresh simulations, invalid/unknown lookup and destruction, live lookup, removal, surviving entities, replacement ID 3, and failed destruction through an old ID after replacement. Maximum-ID overflow protection verified by source inspection, not runtime exhaustion testing.
- Next action: Begin the next foundation task. A fixed-tick command queue can now target stable entity IDs without exposing EnTT handles.

## Acceptance criteria

- [x] GameEntityId is a distinct equality-comparable 64-bit value type; zero represents an invalid ID.
- [x] Simulation creates entities with unique, nonzero, monotonically allocated IDs independent of EnTT handles; exhausted IDs are rejected before wraparound.
- [x] Existence lookup and destruction use stable IDs without exposing registry mutation; destroyed IDs are not reused and unknown IDs are handled safely.
- [x] Headless tests verify deterministic allocation, lookup, destruction, and non-reuse.
- [x] Debug build and all tests pass.
