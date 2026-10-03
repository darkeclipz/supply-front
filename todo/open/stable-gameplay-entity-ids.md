# Add stable gameplay entity IDs

## Description

Give simulation entities permanent identities independent of EnTT handles, as required by docs/SYSTEM-DESIGN.md. Implement simulation-owned creation, existence lookup, and destruction with monotonically allocated IDs that are never reused within a simulation. Keep the registry private and leave GameSession mutation APIs and serialization for later command/save tasks.

Tutor checkpoint: active.

- Current step: Private stable-ID index and entity_exists verified. Add destroy_entity and lifecycle/non-reuse test; proposed, not implemented.
- Context: Simulation indexes uint64_t ID values to private EnTT handles in m_entities_by_id. create_entity inserts into the index inside its cleanup-protected try block. entity_exists uses contains. Next add [[nodiscard]] bool destroy_entity(GameEntityId id): find mapping, return false if absent, destroy mapped EnTT entity, erase mapping, return true. Do not decrement m_last_entity_id. Registry stays private; GameSession mutation and serialization remain later tasks.
- Verification: On 2026-10-03, agent reviewed index insertion, query, and headless test; cmake --build --preset debug -j 2 succeeded and ctest --preset debug passed all 23 tests. Unknown/zero lookup and both live IDs verified. Destruction/non-reuse unimplemented. Overflow guard previously source-reviewed, not runtime-tested.
- Next action: Programmer adds destroy_entity declaration/definition and one lifecycle test covering invalid/unknown destruction, removal, surviving entity, repeated destruction, newly allocated ID 3, and continued invalidity of old ID. Rebuild Debug and run ctest (24 expected). Review and close todo after all criteria have evidence.
- Blocker: None.

## Acceptance criteria

- [x] GameEntityId is a distinct equality-comparable 64-bit value type; zero represents an invalid ID.
- [x] Simulation creates entities with unique, nonzero, monotonically allocated IDs independent of EnTT handles; exhausted IDs are rejected before wraparound.
- [ ] Existence lookup and destruction use stable IDs without exposing registry mutation; destroyed IDs are not reused and unknown IDs are handled safely.
- [ ] Headless tests verify deterministic allocation, lookup, destruction, and non-reuse.
- [ ] Debug build and all tests pass.
