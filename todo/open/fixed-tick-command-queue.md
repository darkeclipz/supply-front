# Add a fixed-tick command queue

## Description

Introduce deterministic command scheduling using docs/SYSTEM-DESIGN.md: future-tick submission, execution ordered by tick/player/per-player sequence, and recorded accepted/rejected outcomes. Exercise the queue with a development-only DestroyEntityCommand targeting a stable gameplay ID. This fixture is not a player-authorized gameplay action; ownership, visibility, and real gameplay commands follow when their state exists.

Tutor checkpoint: active.

- Current step: Shared Tick.hpp and Command.hpp verified. Add submission result enum, validation, and pending storage; proposed, not implemented.
- Context: Command holds execute_at/player_id/sequence/DestroyEntityCommand payload. Add CommandSubmission { queued, invalid_tick, invalid_sequence } in Command.hpp, [[nodiscard]] CommandSubmission submit_command(Command command) to Simulation, vector<Command> m_pending_commands, and unordered_map<uint32_t,uint64_t> m_last_command_sequence. Require execute_at > m_current_tick and sequence greater than the last queued sequence for that player (initially 0). Gaps are allowed; rejected commands do not consume sequence numbers. Use try_emplace(player_id, 0), validate, push_back, then update the sequence value so allocation failure cannot consume a sequence. Submission does not execute payloads or validate target existence; execution later checks targets. tick remains unchanged for this step.
- Verification: On 2026-10-03, agent inspected headers and test include, ran cmake --build --preset debug -j 2 successfully and ctest --preset debug: all 24 tests passed. No scheduling behavior implemented yet.
- Next action: Programmer adds submission enum, declaration, storage, and method, plus one headless test checking queued status without immediate destruction/tick advance, tick 0 rejection, sequence 0 rejection, duplicate/lower sequence rejection, and independent player sequences. Rebuild Debug and run ctest (25 expected). Then implement due-tick execution and recorded outcomes.
- Blocker: None.

## Acceptance criteria

- [x] Command data records execution tick, player ID, per-player sequence, and a stable-ID payload independently of Simulation.hpp.
- [ ] Submission queues commands without immediately changing entities and reports reasons for invalid tick or sequence; per-player sequence identities cannot be duplicated or reused.
- [ ] Each tick executes only commands due for that tick in deterministic player/sequence order and preserves future commands.
- [ ] Execution records accepted/rejected outcomes, including a missing-entity reason, using the development-only destruction fixture.
- [ ] GameSession forwards command submission and exposes read-only results without exposing Simulation or registry mutation.
- [ ] Headless tests verify timing, ordering across players, sequence validation, future retention, missing targets, and equivalent outcomes across scheduler frame partitions and pause/speed changes.
- [ ] Debug build and all tests pass.
