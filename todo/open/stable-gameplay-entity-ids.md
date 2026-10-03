# Add stable gameplay entity IDs

## Description

Give simulation entities permanent identities independent of EnTT handles, as required by docs/SYSTEM-DESIGN.md. Implement simulation-owned creation, existence lookup, and destruction with monotonically allocated IDs that are never reused within a simulation. Keep the registry private and leave GameSession mutation APIs and serialization for later command/save tasks.

Tutor checkpoint: active.

- Current step: Allocation and direct <limits> include verified. Add a private stable-ID index and entity_exists query; proposed, not implemented.
- Context: Add std::unordered_map<std::uint64_t, entt::entity> m_entities_by_id to Simulation, indexed by GameEntityId::value. Insert the mapping inside create_entity's existing try block after attaching the ID component, so failed insertion destroys the unfinished entity and leaves the last-ID counter unchanged. Add [[nodiscard]] bool entity_exists(GameEntityId id) const, implemented using contains(id.value). Use the index only for lookup, not gameplay iteration order. Destruction follows after lookup verification; keep GameSession mutation APIs out of scope.
- Verification: On 2026-10-03, agent confirmed <limits> replaced <climits>, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 22 tests passed. Allocation overflow guard is source-reviewed; lookup is not implemented or verified yet.
- Next action: Programmer adds unordered_map include, index member, entity_exists declaration/definition, and index insertion in create_entity. Add a headless test checking invalid zero, unknown 999, and both created IDs; rebuild Debug and run ctest (23 expected). Then add destruction and non-reuse tests.
- Blocker: None.

## Acceptance criteria

- [x] GameEntityId is a distinct equality-comparable 64-bit value type; zero represents an invalid ID.
- [x] Simulation creates entities with unique, nonzero, monotonically allocated IDs independent of EnTT handles; exhausted IDs are rejected before wraparound.
- [ ] Existence lookup and destruction use stable IDs without exposing registry mutation; destroyed IDs are not reused and unknown IDs are handled safely.
- [ ] Headless tests verify deterministic allocation, lookup, destruction, and non-reuse.
- [ ] Debug build and all tests pass.
