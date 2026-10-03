# Add stable gameplay entity IDs

## Description

Give simulation entities permanent identities independent of EnTT handles, as required by docs/SYSTEM-DESIGN.md. Implement simulation-owned creation, existence lookup, and destruction with monotonically allocated IDs that are never reused within a simulation. Keep the registry private and leave GameSession mutation APIs and serialization for later command/save tasks.

Tutor checkpoint: active.

- Current step: Define a distinct GameEntityId value type; proposed, not implemented.
- Context: include/sim/Simulation.hpp contains a private, currently unused registry and tick counter. No gameplay entity identity or lifecycle API exists. Use include/sim/GameEntityId.hpp, a struct holding std::uint64_t value, zero for invalid/unassigned, and defaulted equality. Fresh simulations will allocate from 1; identical creation sequences produce identical IDs. IDs are scoped to one simulation. Preserve snake_case methods and m_ members.
- Verification: Source inspected on 2026-10-03. No checks run for the new task; the preceding task's Debug build and 21 tests passed.
- Next action: Programmer creates include/sim/GameEntityId.hpp and includes it from Simulation.hpp, then runs cmake --build --preset debug -j 2 and ctest --preset debug. After review, add simulation-owned allocation and lifecycle behavior with headless tests.
- Blocker: None.

## Acceptance criteria

- [ ] GameEntityId is a distinct equality-comparable 64-bit value type; zero represents an invalid ID.
- [ ] Simulation creates entities with unique, nonzero, monotonically allocated IDs independent of EnTT handles; exhausted IDs are rejected before wraparound.
- [ ] Existence lookup and destruction use stable IDs without exposing registry mutation; destroyed IDs are not reused and unknown IDs are handled safely.
- [ ] Headless tests verify deterministic allocation, lookup, destruction, and non-reuse.
- [ ] Debug build and all tests pass.
