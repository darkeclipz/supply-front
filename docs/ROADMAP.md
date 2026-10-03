# Supply Front Roadmap

Planning reference: 3 October 2026. This roadmap expands the system design into small learning and implementation topics, from the current engine seed to a complete game.

## How to use this roadmap with the tutor

- **Choose a topic:** Refer to its chapter, subchapter, and topic number, such as `2.2.1`; adjacent topics can become one small implementation task.
- **Resume existing work:** Read the active checkpoint in [open todos](../todo/open/) before selecting another topic; continue unfinished work unless deliberately switching tasks.
- **Keep progress in todos:** Use the [tutor skill](../.agents/skills/tutor/SKILL.md) and [todo-management skill](../.agents/skills/todo-management/SKILL.md) for checkpoints and acceptance criteria.
- **Create tasks as needed:** Turn the next concrete step into a todo after checking both todo folders; this roadmap is the learning sequence, not a precreated backlog.
- **Learn through implementation:** The tutor explains the relevant C++ and engine concepts, names the files and symbols, and lets you make the implementation changes.
- **Verify before advancing:** Finish the chapter checkpoint through observable gameplay or meaningful headless checks; supplying an example does not complete a topic.
- **Follow dependencies:** Read chapters in order, but introduce a later chapter's smallest necessary feature when an earlier playable checkpoint depends on it.
- **Build feedback early:** Add a basic inspection panel when introducing a system; Chapters 22–23 integrate and improve feedback already present in earlier builds.
- **Treat values as tuning data:** Numerical rules below come from the current design; validate them through prototypes rather than treating them as final balance.
- **Keep decisions explicit:** Record unresolved rules and prototype evidence in the relevant todo; revise the design and roadmap when an agreed decision changes the sequence.

## Design references and current starting point

- **Gameplay authority:** [Game Design Document](gdd/Supply_Front_Game_Design_Document.md) defines the intended player experience, rules, roster, and validation goals.
- **Architecture authority:** [System Design](SYSTEM-DESIGN.md) defines module ownership, fixed ticks, physical goods, command boundaries, and staged implementation gates.
- **Engine reference:** [Template documentation](../TEMPLATE.md) explains the existing rendering seed, dependencies, resource ownership, and build presets; inspect source for current behavior.
- **Inspiration reference:** [Example Design](EXAMPLE-DESIGN.md) supplies useful examples; its rail-first suggestions and deferred-power advice do not override Supply Front's design.
- **Existing presentation:** The seed has a raylib 3D window, orbit camera, model cache, scene ECS, inspector, and scene JSON snapshots; gameplay picking and RTS controls remain ahead.
- **Existing simulation:** `Simulation` owns a private registry and tick counter; `SimulationScheduler` provides fixed 10 Hz ticks, pause, remainder preservation, and integer speeds from 1 through 16.
- **Existing session:** `GameSession` privately owns simulation and scheduler; gameplay systems, commands, match snapshots, and complete replay checks remain planned work.
- **Current continuation:** [Simulation speed controls](../todo/open/simulation-speed-controls.md) still need session forwarding and desktop 1x/2x/4x controls at this planning snapshot.
- **Recorded foundations:** [Headless simulation](../todo/closed/headless-simulation-foundation.md) and [session ownership](../todo/closed/extract-game-session.md) are closed; consult their evidence before repeating work.
- **Status convention:** The chapters describe the intended path, including foundations worth understanding; actual completion and the latest verification belong to the individual todos.

## Playable milestones

| System-design stage | Chapters | Build you should reach | Gate before moving on |
| --- | --- | --- | --- |
| 0 — Foundation | 1–7 | RTS camera and selection over a headless, command-driven graybox world. | Seeded commands reproduce state checksums across frame partitions and offline speed changes. |
| 1 — Local economy harness | 8–12 | Preplaced industries, local stock, power, construction state, research, and unit queues. | Inputs, outputs, claims, and explicit losses reconcile; every stopped process explains its constraint. |
| 2 — Complete economy sandbox | 13–17 | Trucks build and supply an economy from finite starting reserves. | A loaded delivery survives disruption without duplicated stock, leaked claims, or unexplained cargo loss. |
| 3 — Combat supply prototype | 18–20 | A prepared army fights, spends supply, replenishes, and retreats after a raid. | The seeded cut leaves at least 60 seconds of effective local fighting stock and one reachable fallback. |
| 4 — Objective match | 21 | One graybox map, scripted enemy, capture, sectors, and the full offline 1v1 victory loop. | A tester starts production, sustains an attack, recovers after a raid, and can finish the match. |
| 5 — Integrated feedback | 22–23 | Causal panels, route overlay, forecasts, grouped alerts, and accessible controls. | Proposed gates: 80% resolve a shortage within 30 seconds; median stable logistics attention stays below 35%. |
| 6 — Vertical slice | 24–28 | Operational AI, save/replay, River Crossing tutorial, multiplayer trial, and measured stress scenes. | Interrupted deliveries survive persistence; AI obeys visibility; tutorial and performance targets are evaluated. |
| 7 — Production decision | 29–31 | Evidence-backed scope, readable art/audio, focused content, and a packaged PC build. | Expand only when players can fight while automation runs, explain raid consequences, and improve their network. |
| Conditional extensions | 32 | Selected experiments beyond the baseline game. | Each extension needs a concrete player benefit and evidence that the baseline can support it. |

## Chapter 1: Repository orientation and build habits

### 1.1 Understand the project you already have

- **1.1.1 Product loop:** Trace finite deposits → production → truck delivery → depot supply → army → territory and victory.
- **1.1.2 Repository map:** Locate `include/`, `src/`, `tests/`, `assets/`, `cmake/`, `docs/`, and the open and closed todo folders.
- **1.1.3 Runtime modules:** Identify the current `engine_scene`, `engine_render`, `supply_sim`, `supply_session`, and `sandbox` targets and their responsibilities.
- **1.1.4 Seed versus game:** Distinguish demo transforms and spinning models from authoritative gameplay entities and the new simulation clock.

### 1.2 Build and inspect with confidence

- **1.2.1 Debug workflow:** Learn `cmake --preset debug`, `cmake --build --preset debug`, and `ctest --preset debug` using the existing presets.
- **1.2.2 Headless workflow:** Use the `headless-tests` configure, build, and test presets to exercise simulation without a graphics window.
- **1.2.3 Release workflow:** Understand the `release` preset, disabled inspector, staged assets, and why release behavior also needs checking.
- **1.2.4 Evidence and diagnostics:** Read compiler warnings, failing assertions, and startup logs; distinguish a window-system failure from a simulation failure.

### 1.3 Establish learning and coding habits

- **1.3.1 C++ conventions:** Preserve existing simulation conventions: `snake_case` methods and constants, `m_` members, explicit ownership, and const read access.
- **1.3.2 Headers and sources:** Learn declarations, definitions, namespaces, includes, and target dependencies while extending the existing include/source split.
- **1.3.3 Small changes:** Make one behavior observable before expanding it; inspect diffs and preserve unrelated work already in the working tree.
- **1.3.4 Meaningful checks:** Test rules and failure cases that protect gameplay invariants; use desktop observation for camera, picking, and presentation behavior.

### 1.4 Chapter checkpoint

- **1.4.1 Working result:** You can build the seed, locate simulation ownership, and identify the active tutor task without relying on a previous conversation.
- **1.4.2 Verification:** Run the appropriate existing build and tests; record commands, outcomes, and any unavailable desktop verification in the active todo.

## Chapter 2: Game session and simulation time

### 2.1 Understand the existing fixed clock

- **2.1.1 Tick ownership:** Follow `sim::Simulation::tick()` and `current_tick()`; one authoritative tick represents 100 ms at 10 Hz.
- **2.1.2 Frame accumulation:** Follow `SimulationScheduler::advance()` from elapsed nanoseconds through accumulated time to zero or more complete ticks.
- **2.1.3 Pause behavior:** Preserve partial tick progress while ignoring paused elapsed time; pausing must not advance recipes, travel, or scoring.
- **2.1.4 Timing separation:** Keep raylib frame time and demo animation outside gameplay simulation; render frames never choose a variable gameplay step.

### 2.2 Finish the current speed-control task

- **2.2.1 Session forwarding:** Extend `GameSession::advance()` with a defaulted speed argument and forward it to the scheduler without exposing mutable simulation.
- **2.2.2 Desktop controls:** Offer 1x, 2x, and 4x selections and show the chosen speed; retain the scheduler's existing valid integer range of 1–16.
- **2.2.3 Independent pause:** Keep pause separate from speed selection and preserve accumulated progress when changing speed or resuming.
- **2.2.4 Boundary validation:** Preserve rejection of negative elapsed time and invalid speeds, including while paused; avoid unchecked duration arithmetic as timing grows.

### 2.3 Prepare time for gameplay systems

- **2.3.1 Tick timers:** Express build times, recipe cycles, reloads, delays, and capture progress as integer tick counts derived from definition data.
- **2.3.2 Fractional rates:** Use integer numerators and carry accumulators for fractional fuel, production, and service rates without fractional cargo.
- **2.3.3 Session lifecycle:** Define new-match initialization and clean teardown; a restarted match must not inherit old tick remainders or player state.
- **2.3.4 Catch-up policy:** Decide how long frame stalls are handled without silently dropping authoritative gameplay time or creating an unresponsive application.

### 2.4 Chapter checkpoint

- **2.4.1 Working result:** The desktop exposes pause and 1x/2x/4x gameplay speeds while every authoritative update remains a fixed 100 ms tick.
- **2.4.2 Verification:** Check frame-partition equivalence, speed changes, pause/remainder behavior, and approximately 10/20/40 ticks per wall-clock second on desktop.

## Chapter 3: Authoritative entities and deterministic state

### 3.1 Model simulation entities

- **3.1.1 Stable identity:** Introduce persistent `GameEntityId` values; use EnTT handles only for internal lookup and never as saved or replayed identity.
- **3.1.2 Simple components:** Start with fixed-point position, owner, health, and definition references; keep behavior in systems rather than component methods.
- **3.1.3 Entity lifecycle:** Centralize creation, lookup, destruction, and invalid-reference handling so orders and future jobs can safely reference entities.
- **3.1.4 Ownership boundaries:** Keep authoritative entities inside `supply_sim`; create presentation transforms from observations rather than sharing the mutable registry.

### 3.2 Make state repeatable

- **3.2.1 Fixed precision:** Choose and document position and distance units, such as integer millimeters; convert to floats only for display and interpolation.
- **3.2.2 Stable ordering:** Process entities and competing operations with explicit stable-ID tie-breakers instead of depending on unordered container iteration.
- **3.2.3 Seeded randomness:** Add a recorded seed and serializable RNG state; gameplay randomness must not depend on frame rate or wall-clock time.
- **3.2.4 Canonical checksums:** Hash authoritative fields in a stable order, excluding graphics caches, camera state, and incidental runtime handles.

### 3.3 Establish the system update pipeline

- **3.3.1 Tick phases:** Follow the system design's command, topology, production, dispatch, movement, combat, service, cleanup, and observation phases.
- **3.3.2 Scheduled work:** Run dispatch once per second or on material route events while movement and combat remain on the fixed simulation clock.
- **3.3.3 Lifecycle cleanup:** Apply removals and release dependent state at defined boundaries so destruction never invalidates an active system iteration.
- **3.3.4 Headless runner:** Advance a seeded fixture by a specified tick count and print a checksum and relevant state without initializing raylib.

### 3.4 Chapter checkpoint

- **3.4.1 Working result:** A small world can create, update, inspect, and remove gameplay entities through a stable headless simulation.
- **3.4.2 Verification:** Repeat a seeded run and compare checksums; stale IDs fail safely, and different elapsed-time partitions reach the same tick state.

## Chapter 4: Commands, events, and player observations

### 4.1 Accept intent through commands

- **4.1.1 Command envelope:** Record target tick, player ID, per-player sequence, command type, and payload; define deterministic ordering and duplicate handling.
- **4.1.2 Validation results:** Return an explicit accepted or rejected result with a reason for ownership, visibility, range, placement, or stock failures.
- **4.1.3 Session submission:** Expose command submission through `GameSession`; keep input handling and future AI out of direct component mutation.
- **4.1.4 Command fixtures:** Start with minimal legal spawn/setup fixtures and move intent, then add placement, policies, queues, stances, and research as systems arrive.

### 4.2 Publish facts and filtered state

- **4.2.1 Simulation events:** Emit completed facts such as a produced batch, delivered load, failed route, capture, or victory at a defined tick boundary.
- **4.2.2 Observation model:** Provide own-player state through immutable values; design the player-filtered interface now and implement full vision filtering in Chapter 19.
- **4.2.3 Hidden information:** Keep exact enemy inventory, queues, and claims out of playable observations; isolate omniscient inspection as an explicit debug mode.
- **4.2.4 Event consumers:** Let UI, audio, and diagnostics react to events while authoritative state remains the source of truth.

### 4.3 Connect presentation without surrendering authority

- **4.3.1 Read-only rendering:** Build draw data from observations and resolve stable entity IDs without exposing simulation mutation to rendering code.
- **4.3.2 Visual interpolation:** Interpolate previous and current observed positions for smooth visuals; never feed interpolated coordinates back into gameplay.
- **4.3.3 Rejection feedback:** Show the player's failed intent and actionable reason rather than silently ignoring an invalid click or route edit.
- **4.3.4 Command history:** Record commands and their outcomes for debugging; prepare the canonical command log used by later replay work.

### 4.4 Chapter checkpoint

- **4.4.1 Working result:** A human controller can submit a legal command and inspect the resulting observed state; invalid commands explain their rejection.
- **4.4.2 Verification:** Replay the same seeded command sequence headlessly and confirm stable ordering, rejection behavior, and matching authoritative checksums.

## Chapter 5: Definitions, validation, and scenario data

### 5.1 Separate content from runtime state

- **5.1.1 Definition database:** Introduce validated item, recipe, building, unit, weapon, tuning, and map definitions with stable IDs and a content version.
- **5.1.2 Ten cargo types:** Define iron ore, copper ore, crude oil, metal, wire, parts, electronics, fuel, ammunition, and construction kits.
- **5.1.3 Runtime references:** Store definition IDs in components rather than copying recipes, build costs, or weapon rules into every entity.
- **5.1.4 Content fingerprint:** Compute a stable definition hash for fixtures, saves, replays, and future multiplayer compatibility checks.

### 5.2 Validate content before a match starts

- **5.2.1 Structural checks:** Reject missing fields, duplicate IDs, invalid enum values, and unknown references with a path to the offending definition.
- **5.2.2 Numerical checks:** Reject nonpositive cycle times, negative costs, impossible capacities, and rates that cannot be represented by chosen tick arithmetic.
- **5.2.3 Cross-definition checks:** Validate recipe inputs/outputs, building recipe compatibility, unit producers, starting loads, and referenced cargo types.
- **5.2.4 Useful failure messages:** Report all practical startup issues together so fixing one malformed definition does not hide the next obvious error.

### 5.3 Create reproducible maps and fixtures

- **5.3.1 Map schema:** Describe bounds, terrain, deposits, spawn areas, roads, bridges, cable hints, and future sector locations with stable IDs.
- **5.3.2 Small fixtures:** Create focused maps for one recipe, one route, a constrained bridge, and a depot before building the full reference battlefield.
- **5.3.3 Scenario setup:** Define seeded initial entities and stocks; keep test-only setup clearly separate from ordinary player resource creation.
- **5.3.4 Definition reload policy:** Start with restart-to-reload content; add live editing only if it becomes necessary and preserves valid active state.

### 5.4 Chapter checkpoint

- **5.4.1 Working result:** A versioned scenario loads validated definitions and reproducibly initializes a small world without hard-coded gameplay tuning in systems.
- **5.4.2 Verification:** Invalid content fails with useful diagnostics; identical map, content hash, and seed produce identical initial state.

## Chapter 6: World representation and placement foundations

### 6.1 Define the world coordinate contract

- **6.1.1 Ground plane:** Use simulation X/Z coordinates for the overhead 3D ground plane and keep map meters consistent with fixed-point positions.
- **6.1.2 Terrain cells:** Represent walkability, buildability, height/cover metadata, and bounds at a documented grid resolution.
- **6.1.3 Specialized structures:** Keep terrain occupancy, combat navigation, truck roads, cable connectivity, and spatial queries as separate world representations.
- **6.1.4 Nearby queries:** Add a simple spatial index for selection candidates, depot range, sight, and combat; optimize only after measuring representative scenes.

### 6.2 Validate building footprints

- **6.2.1 Footprint occupancy:** Reserve cells for structures and sites; prevent overlap with buildings, blocked terrain, and protected map features.
- **6.2.2 Placement orientation:** Define rotation, footprint origin, docks, and approach points so future road and truck connections are meaningful.
- **6.2.3 Deposit restrictions:** Require mines and pumps to match an available resource deposit rather than placing extraction anywhere.
- **6.2.4 Preview reasons:** Return placement validity and specific blocking causes for a ghost preview without creating an authoritative construction site yet.

### 6.3 Render the first graybox battlefield

- **6.3.1 Terrain drawing:** Show bounds, elevations where needed, deposits, and simple building silhouettes without depending on finished assets.
- **6.3.2 Corridor layout:** Establish a short exposed corridor and a longer alternative; mark bridge and sector locations for later gameplay.
- **6.3.3 World change versions:** Track edits that invalidate occupancy, navigation, road routes, or cable components rather than rebuilding everything each frame.
- **6.3.4 Debug overlays:** Draw cells, footprints, IDs, and connectivity when diagnosing world behavior; keep these optional in normal play.

### 6.4 Chapter checkpoint

- **6.4.1 Working result:** A loaded graybox map shows terrain and deposits, answers spatial queries, and explains valid and invalid structure placement.
- **6.4.2 Verification:** Check map bounds, overlapping footprints, rotated placement, deposit matching, and version changes after an edit.

## Chapter 7: RTS camera, picking, and selection

### 7.1 Replace orbit-only interaction with RTS navigation

- **7.1.1 Camera movement:** Add keyboard and pointer panning, strategic zoom, sensible limits, and a stable view of the map ground plane.
- **7.1.2 Input actions:** Map physical keys and buttons to named actions so remapping and UI input capture can be supported consistently.
- **7.1.3 Screen-to-world picking:** Cast the cursor onto terrain or a ground plane; convert the result into simulation coordinates before constructing commands.
- **7.1.4 UI capture:** Suppress gameplay clicks and camera movement when the inspector or another interface owns the relevant input.

### 7.2 Select entities and communicate intent

- **7.2.1 Single selection:** Pick visible gameplay entities by stable ID and display a clear selection outline and basic read-only information.
- **7.2.2 Group selection:** Add drag-box selection, additive selection, and predictable squad handling instead of selecting individual decorative soldiers.
- **7.2.3 Contextual commands:** Convert right-click intent into move, inspect, or later attack/escort commands according to the selected group and target.
- **7.2.4 Intent feedback:** Show destination markers, selected entities, pending commands, and invalid actions so the first graybox interaction is understandable.

### 7.3 Complete the foundation interaction loop

- **7.3.1 Control groups:** Assign, recall, and focus groups without storing mutable registry handles in UI state.
- **7.3.2 Basic motion probe:** Add minimal deterministic movement in a clear test area; full terrain navigation and formations remain in Chapter 18.
- **7.3.3 Session controls:** Keep pause and speed available while inspecting entities, and expose current tick and seed in optional debug information.
- **7.3.4 Snapshot rendering:** Confirm selected objects and movement are drawn from observation data rather than inspector edits to gameplay state.

### 7.4 Chapter checkpoint

- **7.4.1 Playable result:** You can navigate a two-corridor graybox map, select a group, and issue commands through the session boundary.
- **7.4.2 Foundation gate:** The same seeded commands yield matching state checksums under different frame partitions and pause/speed schedules at equal simulation ticks.

## Chapter 8: Inventories, transactions, and the physical ledger

### 8.1 Store physical cargo locally

- **8.1.1 Inventory holders:** Give buildings, sites, trucks, units, and crates explicit inventories; deposits remain unextracted resource quantities.
- **8.1.2 Capacity rules:** Count each cargo unit as one slot and support both total storage limits and per-item production buffer limits.
- **8.1.3 Cargo permissions:** Restrict endpoint acceptance by definition, including depots accepting only fuel, ammunition, and parts.
- **8.1.4 Stock views:** Distinguish physical, claimed, available, and in-transit stock; a global HUD total never authorizes spending.

### 8.2 Move goods through one transaction API

- **8.2.1 Atomic transfers:** Validate source quantity, destination permission, and capacity before applying a complete stock change.
- **8.2.2 Explicit creation and consumption:** Label extraction, recipe output, build costs, fuel use, weapon use, and destruction losses with their cause.
- **8.2.3 Claim representation:** Represent stock and space claims as references to physical goods or capacity, never as additional inventory.
- **8.2.4 Bounded arithmetic:** Reject underflow, overflow, negative availability, and partially committed operations when an operation cannot complete.

### 8.3 Account for every cargo unit

- **8.3.1 Conservation ledger:** Reconcile initial stock, extraction, conversions, physical holders, and explicit sinks separately for each cargo type.
- **8.3.2 Pending output space:** Reserve complete recipe outputs before a cycle starts so concurrent deliveries cannot occupy their future buffer space.
- **8.3.3 Crate state:** Introduce physical salvage/cancellation crates with cargo, owner/access policy, position, and stable identity.
- **8.3.4 Inventory inspector:** Display local stock and capacity with claim causes so an unavailable item is understandable before trucks exist.

### 8.4 Chapter checkpoint

- **8.4.1 Working result:** Headless commands transfer and consume cargo through validated local transactions and expose an exact ledger.
- **8.4.2 Verification:** Exercise competing claims, full destinations, failed transfers, pending outputs, and explicit losses without negative stock or duplicated goods.

## Chapter 9: Finite extraction and production chains

### 9.1 Extract finite raw resources

- **9.1.1 Extractor association:** Bind iron mines, copper mines, and oil pumps to matching finite deposits and local output inventories.
- **9.1.2 Extraction rate:** Begin with 60 raw units per minute, advancing through tick-based rate accumulation and available output space.
- **9.1.3 Depletion behavior:** Stop extraction when the deposit is empty and expose remaining quantity; warn at 25% of its starting reserve.
- **9.1.4 Powered operation:** Make extraction require local power when Chapter 10 arrives; use an explicit powered test fixture during this chapter.

### 9.2 Implement recipe cycle lifecycle

- **9.2.1 Cycle start:** Require the complete local input batch and claimed output capacity, then consume the inputs exactly once.
- **9.2.2 Cycle completion:** Advance integer progress and create the entire output batch only when the configured cycle finishes.
- **9.2.3 Paused cycles:** Preserve consumed work during power loss; full output, missing input, or disabled operation prevents a new cycle.
- **9.2.4 Recipe selection:** Let a machine shop choose parts or kits, with explicit behavior for a requested change while a cycle is running.

### 9.3 Assemble the complete short economy

- **9.3.1 Metal and wire:** Implement `2 iron ore → 1 metal` and `1 copper ore → 2 wire`, each on a two-second cycle.
- **9.3.2 Parts and kits:** Implement `2 metal → 1 part` and `2 metal + 1 part → 1 kit`, each on a four-second cycle.
- **9.3.3 Advanced outputs:** Implement electronics in five seconds, fuel in two seconds, and ammunition in two seconds with the GDD recipes.
- **9.3.4 Buffers and feedback:** Apply 30-per-input and 60-per-output processor buffers; show selected recipe, cycle progress, rates, and the current stop reason.

### 9.4 Chapter checkpoint

- **9.4.1 Working result:** A preplaced, seeded-input harness produces every cargo type and visibly stops when input, output space, or operating permission is missing.
- **9.4.2 Verification:** Check exact cycles, finite extraction, recipe changes, interrupted work, and ledger reconciliation over thousands of ticks.

## Chapter 10: Local power and generator fuel

### 10.1 Build connected cable networks

- **10.1.1 Cable representation:** Model cables and building attachment points separately from roads and terrain navigation.
- **10.1.2 Connectivity components:** Group attached sources and consumers into local networks and recompute affected components after edits or destruction.
- **10.1.3 Connection rules:** Prototype cable reach, placement, and attachment feedback before committing to a detailed construction rule.
- **10.1.4 Network inspection:** Show generation, connected demand, consumer priority, and disconnected buildings on a simple power overlay.

### 10.2 Run fueled generation and prioritize demand

- **10.2.1 Generator inventory:** Give each diesel generator 60 local fuel slots and a delivery demand; it starts with no free fuel.
- **10.2.2 Generation budget:** Begin with 10 power supply while running and deterministic consumption of 6 fuel per minute.
- **10.2.3 Consumer allocation:** Apply draw 1 for mines/smelters/refineries and draw 2 for other production/research, using stable player-set priorities.
- **10.2.4 Explicit shutdown:** Empty or switched-off generators provide no power and consume no fuel; nonpowered storage, loading, training, and depot service continue.

### 10.3 Protect the startup and recovery loop

- **10.3.1 Cycle preservation:** Pause affected production and research without consuming inputs twice or losing already completed progress.
- **10.3.2 Oil dependency:** Require generator power for pumps and refineries so sustained fuel production must be established before reserves run out.
- **10.3.3 Startup delivery seam:** Prepare the declared warehouse-to-generator connection; seed generator inventory only in explicitly labeled local harness fixtures.
- **10.3.4 Fuel diagnostics:** Show local fuel endurance, inbound supply, and the consumers stopped by a shortage or disconnected cable.

### 10.4 Chapter checkpoint

- **10.4.1 Working result:** Fuel powers a local industrial chain, and the player can shed demand or repair connectivity with clear feedback.
- **10.4.2 Verification:** Check disconnect/reconnect, priority shortages, manual shutdown, fuel exhaustion, and paused cycles with exact stock conservation.

## Chapter 11: Construction sites and recovery rules

### 11.1 Turn placement into construction state

- **11.1.1 Site command:** Validate footprint, ownership, required deposit, and engineer/HQ eligibility before creating a site with a definition reference.
- **11.1.2 Kit demand:** Request kits from an eligible selected warehouse; keep undelivered claims separate from kits physically stored at the site.
- **11.1.3 Assembly progress:** Advance construction only after the required kits arrive and the chosen builder can legally work on the site.
- **11.1.4 Completion transition:** Replace the site with its finished building while preserving stable references or explicitly updating dependents.

### 11.2 Handle interruptions and cancellation

- **11.2.1 Builder interruption:** Define what happens when an engineer moves away, is destroyed, or receives another order; expose the waiting reason.
- **11.2.2 Site cancellation:** Release undelivered claims and turn already delivered kits into a physical crate rather than refunding them globally.
- **11.2.3 Site destruction:** Resolve stock loss and dependent delivery jobs through explicit lifecycle rules and ledger entries.
- **11.2.4 Construction panel:** Show kit cost, delivered/claimed quantity, assigned builder, expected duration, and the next missing condition.

### 11.3 Implement bounded recovery

- **11.3.1 HQ fallback assembly:** Allow the HQ to assemble nearby sites using an adjacent warehouse if no engineer survives; document the prototype range decision.
- **11.3.2 Emergency workshop:** Track the per-match maximum of 6 kits and 6 parts, available only when the relevant stocks and active production are empty.
- **11.3.3 Road construction seam:** Prepare immediate engineer-placed dirt paths and kit-funded pavement upgrades for the road graph chapter.
- **11.3.4 Definition-driven costs:** Use GDD structure kit costs and build times as data; keep unresolved site and repair details explicit in the relevant task.

### 11.4 Chapter checkpoint

- **11.4.1 Working result:** A seeded local harness can assemble and cancel a site; ordinary sites request kits and wait until Chapter 17 integrates delivery.
- **11.4.2 Verification:** Check cancellation before/after delivery, lost engineers, fallback eligibility, completion ownership, and exhausted emergency allowances.

## Chapter 12: Research and unit production

### 12.1 Introduce local production queues

- **12.1.1 HQ queues:** Produce infantry and engineers from local parts; avoid inventing a global stock pool for training costs.
- **12.1.2 Factory queues:** Produce scouts, trucks, tanks, and artillery from local parts/electronics with the GDD costs and build times.
- **12.1.3 Queue start:** Consume the complete recipe at production start and reserve the conditions needed to finish and release the unit.
- **12.1.4 Factory buffers:** Apply 60-per-recipe-input buffers and 120 each of fuel/ammunition for vehicle starting loads.

### 12.2 Unlock and release equipped units

- **12.2.1 Research lifecycle:** Consume 12 electronics and 8 parts locally and advance a powered 90-second research operation.
- **12.2.2 Faction unlocks:** Apply completed tank/artillery research to the owning player's producers while keeping unsupported queue entries invalid.
- **12.2.3 Starting supplies:** Transfer listed fuel and ammunition from producer inventory before a unit leaves; show the unit waiting when loads are missing.
- **12.2.4 Spawn clearance:** Choose a valid exit position and rally intent without placing a new unit inside buildings or beyond map bounds.

### 12.3 Make queue changes conserve goods

- **12.3.1 Cancellation refund:** Return 80% of each consumed input rounded down, create crates for overflow, and record the remainder as an explicit sink.
- **12.3.2 Producer destruction:** Define losses for active work, unstarted queue entries, starting loads, and existing producer inventory.
- **12.3.3 Queue diagnostics:** Show missing local inputs, research requirements, power, starting loads, and exit clearance separately.
- **12.3.4 Local economy harness:** Operate the preplaced extraction/recipe/power/research chain through normal commands while transport remains under construction.

### 12.4 Chapter checkpoint

- **12.4.1 Playable result:** The local harness turns inputs into researched, supplied unit instances and explains every waiting queue or stopped process.
- **12.4.2 Local-economy gate:** Verify no negative stock, duplicate claims, buffer overflow, or unexplained goods; construction requests remain pending until delivery exists.

## Chapter 13: Storage policies and declared logistics

### 13.1 Define the player-controlled network

- **13.1.1 Directed connections:** Store a source, destination, permitted cargo, priority, optional corridor, and danger permission for each declared link.
- **13.1.2 Endpoint compatibility:** Reject incompatible cargo, enemy endpoints, missing entities, and connections that cannot legally serve the destination.
- **13.1.3 Direct and relayed supply:** Allow declared processor-to-factory delivery and warehouse relays without inferring unwanted transfers.
- **13.1.4 Cycle prevention:** Detect automatic relay cycles and refuse policies that create endless warehouse transfer loops.

### 13.2 Implement storage and demand policy

- **13.2.1 Storage capacities:** Begin with 2,000 slots for warehouses and 600 for depots; validate the total of per-item targets against capacity.
- **13.2.2 Distinct thresholds:** Keep source reserve floor, destination replenishment minimum, and destination target separate in state and UI.
- **13.2.3 Outbound availability:** Offer only unclaimed physical stock above the protected floor; higher dispatch priority never silently spends protected stock.
- **13.2.4 Replenishment deficit:** Below the minimum, request toward target after accounting for usable stock, inbound cargo, and remaining unclaimed space.

### 13.3 Generate understandable offers and requests

- **13.3.1 Production demand:** Derive missing inputs from selected recipes and queued units while subtracting compatible existing inbound reservations.
- **13.3.2 Other demand sources:** Generate fuel, depot, research, construction-kit, and salvage requests through the same endpoint contract.
- **13.3.3 Stable request identity:** Track creation time, class, permitted suppliers, remaining quantity, and the cause so planners and alerts refer to the same request.
- **13.3.4 Policy editing:** Apply floor, target, and connection edits through commands; reconcile pending demand rather than generating duplicate requests.

### 13.4 Chapter checkpoint

- **13.4.1 Working result:** A declared network exposes exact offers and deficits and explains when stock is protected, space is claimed, or no supplier is connected.
- **13.4.2 Verification:** Check target capacity, threshold transitions, simultaneous requests, direct supply, and relay-cycle rejection.

## Chapter 14: Roads, corridors, and truck routing

### 14.1 Build the strategic road graph

- **14.1.1 Nodes and edges:** Model intersections, endpoint docks, road segments, permitted directions, speed, condition, and entry capacity with stable IDs.
- **14.1.2 Road surfaces:** Start with dirt at 8 m/s, pavement at 12 m/s, and permitted off-road connectors at 4 m/s.
- **14.1.3 Bridge entities:** Link each damageable bridge to its road edge so destruction closes that edge rather than merely hiding a mesh.
- **14.1.4 Engineer road orders:** Connect dirt placement and kit-paid pavement upgrades to construction and graph version updates.

### 14.2 Find routes that obey player policy

- **14.2.1 Travel-time path search:** Choose permitted routes by estimated journey time using road length, speed, and relevant constraints.
- **14.2.2 Corridor precedence:** Prefer a valid declared corridor; preserve locked policies and never route around a closure through unconstrained terrain A*.
- **14.2.3 Known danger:** Avoid visible enemies and known enemy control by default; require player permission to use an explicitly dangerous corridor.
- **14.2.4 Fog uncertainty:** Allow policy-permitted fogged segments and label their uncertainty without granting the planner hidden enemy information.

### 14.3 Keep routes responsive to the world

- **14.3.1 Route cache keys:** Cache paths against graph and relevant policy versions, then invalidate affected routes after edits or bridge changes.
- **14.3.2 Damage slowdown:** Reduce usable speed before a damaged road becomes blocked, with clear route estimates and visual state.
- **14.3.3 Endpoint access:** Require valid dock approaches and connectors so a reachable road does not imply an unreachable building can be served.
- **14.3.4 Route overlay:** Draw direction, source/destination, corridor choices, blocked edges, and uncertainty before integrating full traffic animation.

### 14.4 Chapter checkpoint

- **14.4.1 Working result:** The player can declare a road delivery corridor, inspect its travel estimate, and see a permitted alternative after an edit.
- **14.4.2 Verification:** Closed bridges cannot be bypassed illegally; locked corridors, graph invalidation, off-road connectors, and known-danger policy behave consistently.

## Chapter 15: Dispatch, reservations, and truck pools

### 15.1 Allocate scarce transport fairly

- **15.1.1 Truck pools:** Associate each available truck with a warehouse pool and distinguish idle, assigned, returning, and direct-control states.
- **15.1.2 Dispatch cadence:** Recompute eligible jobs once per second or after material events instead of searching all supplier pairs every render frame.
- **15.1.3 Priority classes:** Serve emergency defense, frontline supply, production, and reserve filling in that order.
- **15.1.4 Stable tie-breaks:** Within a class prefer the oldest eligible request, then journey time and stable IDs; explain jobs that remain ineligible.

### 15.2 Claim stock, capacity, and a vehicle together

- **15.2.1 Atomic assignment:** Create a job only when source stock, destination capacity, route permission, and one pool truck can all be claimed.
- **15.2.2 Stock exclusivity:** Prevent recipe input, construction, unit production, and shipment claims from spending the same cargo.
- **15.2.3 Destination space:** Subtract inbound cargo and pending recipe outputs from capacity so multiple trucks cannot promise the same free slots.
- **15.2.4 Claim lifecycle:** Define acquire, commit, resize, transfer, and release operations with a clear owner and release reason.

### 15.3 Form useful loads

- **15.3.1 Cargo batching:** Combine permitted cargo up to 40 slots while respecting each item request and the source floor.
- **15.3.2 Partial-load timeout:** Dispatch a partial load after at most 10 seconds of waiting; emergency requests dispatch immediately.
- **15.3.3 Reservation reconciliation:** Adjust jobs after policy, stock, ownership, and route changes without allowing stale claims to survive indefinitely.
- **15.3.4 Planner inspection:** Show request age, priority, selected source, assigned vehicle, claimed quantities, and the reason a load cannot dispatch.

### 15.4 Chapter checkpoint

- **15.4.1 Working result:** Multiple declared destinations receive deterministic jobs without overbooking cargo, destination slots, or trucks.
- **15.4.2 Verification:** Stress simultaneous last-stock claims, mixed loads, full destinations, batching deadlines, priority changes, and release after cancellation.

## Chapter 16: Truck trips, queues, and throughput

### 16.1 Implement the delivery state machine

- **16.1.1 Trip stages:** Model assignment, travel to source, bay queue, loading, travel, edge/bay queues, unloading, safe return, and idle explicitly.
- **16.1.2 Physical loading:** Spend five seconds loading, transfer exact source goods into the manifest, and release the corresponding source claim atomically.
- **16.1.3 Loaded travel:** Advance fixed-point motion along the chosen road route; baseline trucks consume no fuel and carry no weapon ammunition.
- **16.1.4 Physical unloading:** Spend five seconds unloading, transfer the manifest into destination stock, and release the inbound space claim atomically.

### 16.2 Respect constrained roads and docks

- **16.2.1 Directional admission:** Admit one truck per second per direction on ordinary road edges rather than inferring capacity from visual spacing.
- **16.2.2 Shared bridge admission:** Admit one truck every two seconds total on a bridge, using stable queues across both directions.
- **16.2.3 Dock bays:** Give warehouses two bays and producers/depots one; represent queued and servicing trucks without claiming a bay twice.
- **16.2.4 Local spacing:** Allow passage through friendly units and add visual spacing while preserving graph-scheduled travel and throughput accounting.

### 16.3 Return vehicles and expose delivery capacity

- **16.3.1 Safe empty return:** Keep completed trucks unavailable until they reach their pool warehouse or a reachable safe source if the original source is gone.
- **16.3.2 Direct-control handoff:** Suspend automated assignment for emergency control and rejoin the pool after a defined safe return.
- **16.3.3 Measured flow:** Track successful cargo delivered per minute, trip time, utilization, queue delay, and time since last successful delivery.
- **16.3.4 Capacity example:** Reproduce the 600 m paved round trip: 110 seconds with handling and about 21.8 cargo/minute per full truck before queues.

### 16.4 Chapter checkpoint

- **16.4.1 Working result:** Trucks repeatedly load, travel, unload, and return while showing manifests, bay queues, edge queues, and expected throughput.
- **16.4.2 Verification:** Check exact transfers, handling times, directional/bridge admissions, mixed cargo, return availability, and the reference throughput calculation.

## Chapter 17: Delivery disruption and the complete economy sandbox

### 17.1 Repair jobs when the world changes

- **17.1.1 Permitted detour:** Replan a blocked trip through a legal alternative while retaining its manifest and valid destination claim.
- **17.1.2 Loaded retreat:** If no permitted detour exists, release the original inbound claim and claim reachable safe storage for the still-loaded truck.
- **17.1.3 Visible holding state:** Retain cargo when no safe storage has room; expose the blocker and alert rather than discarding cargo or marking the truck idle.
- **17.1.4 Lost endpoints and vehicles:** Repair jobs after destination destruction/capture; log a destroyed truck's manifest as an explicit loss and release all dependent claims.

### 17.2 Integrate construction and finite startup

- **17.2.1 Starting base:** Spawn the HQ, engineer squad, generator, warehouse, three trucks, and infantry beside starter deposits.
- **17.2.2 Starting reserves:** Put 60 kits, 30 parts, 20 electronics, 120 fuel, and 160 ammunition in the warehouse, with an initially empty generator.
- **17.2.3 Startup fuel route:** Initialize the declared warehouse-to-generator connection so ordinary dispatch, loading, and delivery establish the first power supply.
- **17.2.4 Delivered construction:** Complete engineer construction, research inputs, unit inputs, and starting loads through ordinary deliveries rather than test-only stock injection.

### 17.3 Make failure understandable and recoverable

- **17.3.1 Stalled-job detection:** After 15 seconds without progress, identify the blocking stock, bay, edge, route, or storage condition.
- **17.3.2 Grouped alerts:** Report one root cause with affected destinations and available detour, capacity, or policy actions instead of one warning per truck.
- **17.3.3 Lifecycle fixtures:** Close a bridge while loaded or queued, destroy a destination, and cancel construction after delivery; add capture cases when Chapter 21 arrives.
- **17.3.4 Economy opening:** Build the metal/parts/kits/electronics/ammunition chains, establish sustained fuel, and produce a loaded vehicle from finite reserves.

### 17.4 Chapter checkpoint

- **17.4.1 Playable result:** The complete economy sandbox supports an ordinary opening, physical construction, powered production, and automatic deliveries.
- **17.4.2 Economy-sandbox gate:** Goods and manifests reconcile through cancellations and disruptions, while every blocked delivery names its cause and viable next actions.

## Chapter 18: Combat movement, orders, and group behavior

### 18.1 Navigate combat units over terrain

- **18.1.1 Terrain paths:** Use a combat navigation grid distinct from the road graph; honor obstacles, footprints, map bounds, and unit movement rules.
- **18.1.2 Queued path requests:** Budget and cache pathfinding requests so large orders do not make one tick perform unlimited work.
- **18.1.3 Fixed-point travel:** Advance movement by deterministic per-tick distance and carry remainders without frame-rate-dependent speeds.
- **18.1.4 Local avoidance:** Add lightweight friendly separation and stuck detection while keeping group movement readable and reproducible.

### 18.2 Execute orders through state machines

- **18.2.1 Order queue:** Represent move, attack, hold, escort, and return-to-depot intent with stable targets and explicit interruption rules.
- **18.2.2 Queued commands:** Support shift-appended orders and replacement orders; distinguish player queue state from autonomous temporary behavior.
- **18.2.3 Stance policy:** Implement hold, cautious advance, assault, escort, and return behavior with clear target/chase limits.
- **18.2.4 Target loss:** Resolve destroyed, hidden, unreachable, or captured targets safely and communicate why an order ended or changed behavior.

### 18.3 Keep groups useful without excessive micro

- **18.3.1 Arrival slots:** Allocate simple group destinations so a multi-unit move does not order every entity onto the same point.
- **18.3.2 Squad abstraction:** Treat infantry as a five-person squad with shared stock and one gameplay identity; individual models remain presentation detail.
- **18.3.3 Escort execution:** Follow and protect a convoy without changing the convoy's declared delivery policy or truck-pool ownership.
- **18.3.4 Group feedback:** Show queued destinations, stance, escort target, stuck conditions, and group supply summaries as the systems become available.

### 18.4 Chapter checkpoint

- **18.4.1 Working result:** Groups cross the graybox battlefield, follow queued orders, and escort trucks through the same validated command boundary.
- **18.4.2 Verification:** Check obstacles, group arrival, queue replacement, target removal, reproducibility, and that combat navigation cannot bypass truck routing rules.

## Chapter 19: Visibility, targeting, and weapons

### 19.1 Establish fair battlefield information

- **19.1.1 Player visibility:** Track unexplored, explored-but-unseen, and currently visible areas per player at authoritative tick boundaries.
- **19.1.2 Sight and occlusion:** Combine vision radius and line of sight with terrain/building obstruction and spatial candidate queries.
- **19.1.3 Observation filtering:** Reveal visible enemy units and truck activity while withholding exact enemy inventory, queues, and reservations.
- **19.1.4 Known threats:** Feed observed danger into targeting, route policy, and later AI without querying hidden enemy positions.

### 19.2 Resolve weapon behavior

- **19.2.1 Weapon definitions:** Store range, reload, burst cost, projectile speed, accuracy, and damage as prototype tuning data.
- **19.2.2 Eligible targets:** Acquire visible, in-range targets according to stance and stable tie-breaks; define whether an attack can persist after sight is lost.
- **19.2.3 Projectiles and damage:** Resolve projectile travel, hits, terrain cover, and directional armor through deterministic authoritative logic.
- **19.2.4 Artillery rules:** Require a spotter for accurate indirect fire or allow a selected ground location at reduced accuracy.

### 19.3 Connect combat to lifecycle and supply

- **19.3.1 Burst expenditure:** Spend shared ammunition per weapon burst and prevent firing when stock is insufficient.
- **19.3.2 Damage cleanup:** Resolve deaths in a stable phase and invalidate targets, ownership references, orders, and affected logistics jobs.
- **19.3.3 Automatic engagement:** Let units attack eligible visible enemies within their stance without adding repeatable activated abilities.
- **19.3.4 Combat feedback:** Show shots, impacts, damage, cover/armor results, and death clearly enough to tune combat before replacing graybox assets.

### 19.4 Chapter checkpoint

- **19.4.1 Working result:** Units fight visible enemies, spend ammunition, and respect sight, cover, armor, projectile travel, and stance limits.
- **19.4.2 Verification:** Check hidden-target rejection, information filtering, ammunition exhaustion, seeded accuracy, simultaneous deaths, and command replay checksums.

## Chapter 20: Unit roster, depot service, and army endurance

### 20.1 Implement the complete baseline roster

- **20.1.1 Infantry and engineers:** Apply infantry's 20 ammo capacity and engineers' 10 ammo plus 10 repair parts; preserve their capture and construction roles.
- **20.1.2 Scout car:** Start with 20 fuel/12 ammo and maximum moving/firing use of 1 fuel/3 ammo per minute; test scouting and exposed-truck raids.
- **20.1.3 Tank and artillery:** Start tanks at 40 fuel/30 ammo and artillery at 25 fuel/40 ammo, with GDD consumption rates and clear battlefield roles.
- **20.1.4 Supply trucks:** Keep trucks at 40 cargo slots with no baseline fuel/ammo expenditure and no weapons; their vulnerability remains fully simulated.

### 20.2 Spend and replenish local supplies

- **20.2.1 Movement fuel:** Consume fuel only while powered combat vehicles move, using integer accumulation for rates including artillery's 1.5 fuel/minute.
- **20.2.2 Depot service:** Transfer stored fuel, ammo, and parts within 60 m at no more than 120 total cargo/minute across all serviced units.
- **20.2.3 Service priority:** Serve manually selected groups first, then lowest supply percentage and stable ID; aggregate rate limits across cargo types.
- **20.2.4 Engineer repairs:** Spend one carried or nearby-depot part for 10% maximum-health repair, cap at full health, and make access/rate rules explicit.

### 20.3 Implement supply states and response policies

- **20.3.1 Ready and low:** Warn when fuel or ammunition is at or below 25% and show carried stock, reachable depots, and the scarce resource.
- **20.3.2 Exhaustion behavior:** Zero ammo stops firing; zero fuel permits a 15% crawl with no dash, tow, or road bonus and causes no direct starvation damage.
- **20.3.3 Sustain policy:** Optionally return to a safe reachable depot at low stock; resume the previous assignment only when the player enables it.
- **20.3.4 Army supply probe:** Compare idle, moving, and intense-combat demand; stage a stocked offensive followed by a corridor cut and fallback.

### 20.4 Chapter checkpoint

- **20.4.1 Playable result:** The full roster can attack from prepared reserves, exhaust supply predictably, refill at a depot, repair, and withdraw.
- **20.4.2 Combat-supply gate:** A seeded cut leaves at least 60 seconds of effective fighting stock and one reachable fallback; service never exceeds stock or aggregate rate.

## Chapter 21: Objectives, capture, and the first complete 1v1

### 21.1 Build a map that creates operational choices

- **21.1.1 Reference battlefield:** Tune a roughly 2 × 2 km map with modest home resources, larger contested deposits, and two viable ground corridors.
- **21.1.2 Finite reserves:** Size starter deposits for at least 20 minutes of typical use as an initial goal and validate expansion pressure through playtests.
- **21.1.3 Sector capture:** Place three central sectors; uncontested infantry captures in 20 seconds while vehicles alone cannot capture.
- **21.1.4 Victory scoring:** After five opening minutes, holding at least two sectors grants one point/second; 900 points or enemy HQ destruction wins.

### 21.2 Implement capture, salvage, and bridge recovery

- **21.2.1 Storage capture:** Transfer a warehouse/depot after 30 uninterrupted seconds beside it with no defending combat unit within 60 m.
- **21.2.2 Ownership transition:** Preserve surviving stock, reveal it to the captor, reset old routes/claims, and require new friendly connections.
- **21.2.3 Salvage recovery:** Drop 25% of destroyed storage stock into visible crates, record the remainder as loss, and collect through ordinary transport requests.
- **21.2.4 Bridge engineering:** Rebuild a bridge with 4 delivered kits over 45 seconds; deliberate demolition takes 20 seconds and incoming damage cancels it.

### 21.3 Complete an ordinary offline match

- **21.3.1 Legal scripted opponent:** Submit simple economic and combat commands through the same validator; keep any debug scenario reveals explicit.
- **21.3.2 Match lifecycle:** Add start, pause, victory/defeat, final score, restart, and return flow with a clear rule for simultaneous win conditions.
- **21.3.3 Objective feedback:** Keep ownership, capture progress, score, opening-period timing, estimated time to victory, and HQ danger visible.
- **21.3.4 End-to-end situations:** Exercise finite-start production, a forward-depot-supported offensive, and recovery after a corridor raid in ordinary play.

### 21.4 Chapter checkpoint

- **21.4.1 Playable result:** A new tester can complete an offline 1v1 against a scripted opponent through sectors or HQ destruction.
- **21.4.2 Objective-match gate:** Capture interruptions, ownership/claim resets, salvage, scoring timing, bridge recovery, and the three required match situations work together.

## Chapter 22: Causal diagnostics and operational forecasts

### 22.1 Explain the actual limiting stage

- **22.1.1 Typed statuses:** Represent missing input, output full, no power, no truck, bay queue, blocked route, and insufficient depot service as structured causes.
- **22.1.2 Cause tracing:** Follow a stopped factory through its source, claims, truck, edge/bay, and destination capacity to the earliest actionable constraint.
- **22.1.3 Shared model:** Let UI and AI consume the same diagnostics and estimates over their allowed observations rather than rebuilding separate logic.
- **22.1.4 Action links:** Connect a diagnosis to the relevant building, inventory policy, route segment, truck pool, or depot on the map.

### 22.2 Forecast production, delivery, and demand

- **22.2.1 Nameplate versus actual:** Show recipe maximum output separately from recently completed output and explain missing uptime or input.
- **22.2.2 Delivery estimates:** Calculate cargo/minute from capacity, round-trip travel, handling, queues, assigned trucks, and route uncertainty.
- **22.2.3 Army projections:** Estimate selected-composition fuel/ammo demand under idle, movement, and intense-fire assumptions.
- **22.2.4 Reserve endurance:** Estimate time to depletion from usable reserves and projected net draw; display stable when delivery meets demand.

### 22.3 Measure what helps a player act

- **22.3.1 Limiting capacity:** Compare production, transport, and depot-service stages without implying that distant warehouse stock is immediately usable.
- **22.3.2 Shared-stock accounting:** Avoid double-counting reserves or trucks across overlapping army and route forecasts.
- **22.3.3 Grouped notifications:** Merge related failures by root cause, expose time since last success, and separate routine shortage from imminent depletion.
- **22.3.4 Operational history:** Track stock, deliveries, consumption, interruptions, and recovery events for debugging, playtests, and later debriefs.

### 22.4 Chapter checkpoint

- **22.4.1 Working result:** A player can inspect a stopped tank factory, follow its electronics shortage to a blocked route, and identify a useful corrective action.
- **22.4.2 Verification:** Compare estimates with controlled production/trip/service examples and ensure forecasts use only accessible stock and player-visible information.

## Chapter 23: Interface, feedback, and accessibility

### 23.1 Make essential information easy to inspect

- **23.1.1 Main battlefield HUD:** Show score, army/depot/sector priorities, and compact stored/claimed/in-transit cargo without obscuring tactical action.
- **23.1.2 Warehouse and factory panels:** Show policy, free space, claims, bays, queue, local stock, expected completion, and current constraint.
- **23.1.3 Route and army panels:** Show endpoints, cargo, vehicles, travel, throughput, priority, corridor policy, carried supply, demand, and depot access.
- **23.1.4 Logistics overlay:** Draw directional flow, cargo mix, capacity, waiting, blocked, and uncertain segments; aggregate distant traffic visually only.

### 23.2 Reduce repeated work and misleading feedback

- **23.2.1 Route editing:** Support clear source-to-destination gestures, corridor adjustments, and persistent policy settings through commands.
- **23.2.2 Policy copying:** Copy compatible warehouse settings with capacity validation and explain values that cannot be applied.
- **23.2.3 State feedback:** Add distinct graybox visual/audio cues for production stops, convoy arrivals, low supply, capture, and route interruption.
- **23.2.4 Alert focus:** Use persistent map markers, root-cause grouping, cooldowns, and prioritization so combat alerts remain audible and useful.

### 23.3 Support different players and input needs

- **23.3.1 Remapping:** Expose key/button bindings for camera, selection, commands, groups, pause, speed, and overlays with understandable conflict handling.
- **23.3.2 Legibility:** Support scalable text, high-contrast selection, and status icons/text that do not require distinguishing colors.
- **23.3.3 Comfort controls:** Offer reduced camera motion and adjustable alert audio; check interaction at several window sizes and strategic zoom levels.
- **23.3.4 Familiarization probe:** Add a short explanation of routes, stocks, and panels before timing a seeded shortage-resolution exercise.

### 23.4 Chapter checkpoint

- **23.4.1 Playable result:** Players can fight while inspecting and adjusting their logistics with readable panels, persistent policies, and accessible controls.
- **23.4.2 Feedback gate:** Test the proposed 80%-within-30-seconds diagnosis goal and below-35% median stable-operation logistics attention goal after familiarization.

## Chapter 24: Visibility-limited operational AI

### 24.1 Replace scripts with an economic planner

- **24.1.1 Controller boundary:** Read the same filtered observations as a human and submit commands for a later tick with recorded choices.
- **24.1.2 Viable opening:** Establish fuel, metal, kits, parts, and ammunition without debug stock grants or hidden opponent inventory queries.
- **24.1.3 Demand planning:** Choose an army composition, estimate its supply demand, and size production, reserve targets, truck capacity, and depots accordingly.
- **24.1.4 Expansion decisions:** Compare remaining deposits, contested capacity, route time, power, and exposure when selecting new industry locations.

### 24.2 Make operational choices from evidence

- **24.2.1 Sector priorities:** Evaluate objective score pressure, known defenders, travel time, and supply reach before advancing.
- **24.2.2 Corridor protection:** Assign escorts, defend exposed trucks, and use known safer detours when disruption threatens a planned operation.
- **24.2.3 Raids and offensives:** Distinguish a brief raid from a reserve-supported attack; allow urgent defense exceptions without ignoring supplies.
- **24.2.4 Withdrawal and recovery:** Respond to low stock, lost depots, and route cuts by retreating, repairing, or rebuilding a viable network.

### 24.3 Keep behavior testable and understandable

- **24.3.1 Planning budget:** Limit work per planning cycle and use stable ordering so AI does not create unpredictable tick spikes.
- **24.3.2 Difficulty parameters:** Vary reaction delay, planning depth, and risk tolerance while keeping visibility and economic rules consistent.
- **24.3.3 Decision diagnostics:** Expose the selected goal, rejected alternatives, supply assumptions, and next command in debug inspection.
- **24.3.4 Scenario evaluation:** Run seeded openings, raids, detours, depot losses, and low-supply retreats with measurable outcomes.

### 24.4 Chapter checkpoint

- **24.4.1 Playable result:** An operational opponent builds, expands, protects routes, attacks from reserves, and can recover after disruption.
- **24.4.2 Verification:** AI obeys command/visibility rules, shares forecasts with the player model, respects reaction settings, and reproduces recorded decisions.

## Chapter 25: Match snapshots, save/load, and command replay

### 25.1 Serialize complete authoritative matches

- **25.1.1 Save boundary:** Capture state at a completed tick, including content/schema versions, map, seed, RNG, tick, players, and stable entity IDs.
- **25.1.2 Economy state:** Save inventories, deposits, cycles, queues, construction, research, emergency allowances, power networks, and policies.
- **25.1.3 Transport state:** Save routes, source/destination/truck claims, manifests, pool assignments, bay/edge queues, motion, and blocked-trip state.
- **25.1.4 Combat and controller state:** Save orders, weapon timers, projectiles, supply accumulators, visibility, capture, score, and AI plans/state.

### 25.2 Restore without changing the match

- **25.2.1 Reference reconstruction:** Rebuild runtime handles and resolve stable references after entities load; validate dependencies before accepting the restored match.
- **25.2.2 Compatibility policy:** Migrate explicitly supported versions or reject incompatible saves with a useful explanation; never silently drop unsupported state.
- **25.2.3 Safe file workflow:** Adapt the seed's temporary-file replacement approach for match saves and handle failed, incomplete, or malformed loads predictably.
- **25.2.4 Save interface:** Add manual save/load and clear errors; scene-inspector snapshots remain distinct from full gameplay persistence.

### 25.3 Reproduce and diagnose command histories

- **25.3.1 Replay header:** Record initial scenario/content versions, seed, accepted commands by tick, and AI choices with their player sequences.
- **25.3.2 Replay checksums:** Store periodic authoritative checksums and compare them during headless playback across rendering and timing variations.
- **25.3.3 Divergence diagnosis:** Report the first mismatching tick and relevant subsystem/entity differences rather than only a final incorrect hash.
- **25.3.4 Replay inspection:** Support pause, speed, and selected operational history views without allowing playback controls to mutate recorded commands.

### 25.4 Chapter checkpoint

- **25.4.1 Working result:** A loaded, blocked truck resumes with its exact manifest, route state, and claims; a replay reproduces the match outcome.
- **25.4.2 Verification:** Compare uninterrupted and save/resume runs through deliveries, production, capture, RNG, and AI; reject incompatible or malformed snapshots clearly.

## Chapter 26: The River Crossing tutorial and debrief

### 26.1 Teach the working economy before combat

- **26.1.1 Inherited network:** Start with working iron/copper industries beside a river and use the actual production and logistics systems.
- **26.1.2 Electronics shortage:** Ask the player to connect wire supply, assign transport, complete electronics, and produce a scout car.
- **26.1.3 Delayed combat:** Keep combat pressure away until the first delivery succeeds so the player learns causal inspection without simultaneous threats.
- **26.1.4 Depot preparation:** Fill a forward depot with fuel/ammo, introduce the endurance panel, and take a neutral sector with infantry and two tanks.

### 26.2 Teach interruption and multiple valid responses

- **26.2.1 Bridge raid:** Trigger a readable raid that closes the short route while carried stock gives the army time to act.
- **26.2.2 Three responses:** Support escorted bridge repair, redirection along the longer southern road, and withdrawal to the home depot.
- **26.2.3 Prepared offensive:** Ask for a two-minute supply-supported push and capture of an enemy depot whose stock helps the final operation.
- **26.2.4 Optional goals:** Reward truck survival and preserving a combat group without making one solution the only acceptable route through the mission.

### 26.3 Make learning resumable

- **26.3.1 Mission scripting:** Observe events and submit legal scenario actions; keep tutorial instructions separate from authoritative gameplay rules.
- **26.3.2 Restart points:** Offer checkpoints before the bridge raid and final offensive using the complete match snapshot system.
- **26.3.3 Guided inspection:** Support pause, optional hints, map focus, and progressive explanations without hiding the normal logistics panels.
- **26.3.4 Causal debrief:** Pair army supply history with route interruptions and one concrete alternative while acknowledging multiple contributing causes.

### 26.4 Chapter checkpoint

- **26.4.1 Playable result:** The tutorial teaches a production fix, depot preparation, a recoverable bridge cut, and a reserve-supported final push.
- **26.4.2 Verification:** Each of the three raid responses can complete the scenario; restart points preserve state and new testers can explain the supply consequence.

## Chapter 27: Multiplayer architecture trial

### 27.1 Evaluate synchronization from measured behavior

- **27.1.1 Reproducibility audit:** Compare authoritative checksums across target machines/toolchains before relying on deterministic lockstep.
- **27.1.2 Cost measurements:** Measure tick cost, snapshot size, bandwidth, command latency, and the effect of representative combat/logistics loads.
- **27.1.3 Architecture prototype:** Trial lockstep and/or server-authoritative synchronization as needed to resolve the engineering choice.
- **27.1.4 Decision record:** Choose the production model from evidence and record consequences for state format, latency handling, and debugging.

### 27.2 Transport commands and authoritative results

- **27.2.1 Protocol identity:** Define session, player, sequence, tick, content hash, and protocol-version fields with explicit compatibility errors.
- **27.2.2 Command ordering:** Handle duplicate, late, missing, and reordered packets without executing a player action twice.
- **27.2.3 Authority checks:** Validate ownership and legal action on the authoritative side; never trust a client's claimed resources or damage result.
- **27.2.4 Visibility protection:** If sending player snapshots, filter them by observation so hidden enemy stock is not transmitted to that client.

### 27.3 Exercise a small connected match

- **27.3.1 Trial session flow:** Connect two peers or clients, select the reference map, agree on content, and start a seeded 1v1 trial.
- **27.3.2 Competitive timing:** Run continuously at fixed speed without offline pause or accelerated-time controls changing the shared match.
- **27.3.3 Failure behavior:** Define disconnect, timeout, desynchronization, and trial termination behavior before expanding lobby features.
- **27.3.4 Network diagnostics:** Log packet timing, command acceptance, checksums, and correction/resynchronization events under simulated delay and loss.

### 27.4 Chapter checkpoint

- **27.4.1 Working result:** Two participants can complete a small authoritative match trial using the chosen prototype synchronization approach.
- **27.4.2 Verification:** Record latency/loss tests, cross-machine replay results, bandwidth, information exposure, and unresolved networking work before production scope is set.

## Chapter 28: Profiling, stress scenes, and targeted optimization

### 28.1 Measure representative workloads

- **28.1.1 Reference hardware:** Declare the PC, build type, map, seed, and workload used for each reported performance result.
- **28.1.2 Prototype stress budget:** Exercise 200 combat entities, 150 trucks, and 100 industrial/storage buildings across both players.
- **28.1.3 Separate timings:** Measure headless tick time independently from render time, asset work, input, and interface updates.
- **28.1.4 Phase breakdown:** Attribute cost to movement, path requests, visibility, targeting, production, dispatch, cleanup, and observation creation.

### 28.2 Optimize demonstrated bottlenecks

- **28.2.1 Spatial reuse:** Reuse candidate queries, graph caches, and stable world versions when repeated work dominates measured tick cost.
- **28.2.2 Path budgets:** Queue navigation work and avoid repeated replanning for unchanged goals or routes; expose excessive backlog in diagnostics.
- **28.2.3 Conditional algorithms:** Add flow fields, hierarchical navigation, or multithreading only when representative measurements justify their complexity.
- **28.2.4 Presentation scaling:** Batch/cull repeated models and aggregate distant traffic visuals while continuing to simulate every truck and manifest.

### 28.3 Protect behavior while improving performance

- **28.3.1 Regression replays:** Compare representative checksums and ledger outcomes before and after changes to authoritative work ordering.
- **28.3.2 Long sessions:** Look for memory growth, stale claims, orphan jobs, increasing queues, and shutdown failures over extended matches.
- **28.3.3 Load spikes:** Test mass orders, simultaneous deaths, bridge closures, captures, and restored saves as well as steady operation.
- **28.3.4 Larger-match experiment:** Measure the later 2v2 target of 600 combat entities and 300 trucks without presenting it as an achieved baseline guarantee.

### 28.4 Chapter checkpoint

- **28.4.1 Vertical-slice result:** The integrated AI, tutorial, persistence, network trial, and stress scenes have reproducible evidence and visible remaining limits.
- **28.4.2 Verification:** Report measured tick/frame budgets on declared hardware; optimizations preserve gameplay invariants and representative replay outcomes.

## Chapter 29: Playtesting, balance, and the production decision

### 29.1 Collect evidence about the central experience

- **29.1.1 Tester mix:** Include RTS players and factory-game players, observe actual matches, and use short debriefs to explain behavior.
- **29.1.2 Diagnostic success:** Measure whether at least 80% solve a seeded input/route shortage within 30 seconds after familiarization.
- **29.1.3 Attention budget:** Measure combat versus logistics interaction time and the proposed below-35% median logistics share during stable operation.
- **29.1.4 Raid recovery:** Measure warning time, effective fighting stock, available fallback, retreat success, and recovery after a seeded corridor cut.

### 29.2 Tune systems that shape meaningful choices

- **29.2.1 Reserve tuning:** Adjust unit stocks, depot buffers, service limits, and transport capacity until prepared attacks and recoverable cuts both matter.
- **29.2.2 Expansion pressure:** Tune deposits, sectors, and geography so players seek contested capacity or shorter routes instead of indefinitely stacking safe industry.
- **29.2.3 Economy clarity:** Review each intermediate's role, power's attention cost, direct delivery versus warehouses, and persistent policy usefulness.
- **29.2.4 Combat and recovery:** Tune ranges, armor, accuracy, crawl speed, capture/demolition timing, salvage, and rebuilding costs from observed matches.

### 29.3 Decide whether and how to expand

- **29.3.1 Match pacing:** Compare actual openings, first tech, offensives, depletion, and endings with the proposed 30–45 minute match experience.
- **29.3.2 Simplification choices:** Remove or simplify systems that add repeated work without changing recurring operational or composition decisions.
- **29.3.3 Production scope:** Define maps, missions, supported modes/platforms, polish priorities, and remaining multiplayer work from the measured vertical slice.
- **29.3.4 Work estimates:** Estimate effort only after routing, attention, performance, and networking risks are understood and team availability is known.

### 29.4 Chapter checkpoint

- **29.4.1 Decision result:** Players can fight while automation runs, explain the consequence of a raid, and make useful changes to their network.
- **29.4.2 Production gate:** Commit to expansion from playtest and technical evidence; if a core gate fails, revise the relevant earlier system and retest it first.

## Chapter 30: Readable art, animation, and audio polish

### 30.1 Replace graybox assets with readable identities

- **30.1.1 Art direction:** Use grounded, stylized World War II era equipment in fictional regions with clear silhouettes at tactical and strategic zoom.
- **30.1.2 Industrial identities:** Distinguish ore hoppers, wire processing, oil tanks, machine shops, electronics buildings, and visible loading docks.
- **30.1.3 Battlefield hierarchy:** Prioritize combat units, depots, sectors, ownership outlines, and selection over decorative detail.
- **30.1.4 Asset integration:** Reuse the model cache and explicit GPU ownership; introduce shaders, materials, or animation only with a clear presentation need.

### 30.2 Animate useful operational feedback

- **30.2.1 Factory activity:** Show operating, waiting, unpowered, and damaged states without implying internal conveyors or machines that are not simulated.
- **30.2.2 Convoy activity:** Show loading/unloading, loaded movement, queues, retreat, and losses consistently with manifests and trip states.
- **30.2.3 Combat effects:** Add recoil, projectile trails, impacts, smoke, and destruction while preserving road visibility and supply indicators.
- **30.2.4 Zoom-dependent detail:** Replace distant detail with clear icons/flow cues and keep every truck's cargo and combat exposure authoritative.

### 30.3 Build an operational audio mix

- **30.3.1 Distinct cues:** Differentiate production completion, convoy arrival, replenishment, interruption, capture, combat danger, and low supply.
- **30.3.2 Priority and cooldowns:** Give combat and imminent supply failures priority over repetitive factory sounds; speak once per relevant group transition.
- **30.3.3 Spatial ambience:** Fade industrial ambience with camera distance and balance music, voice, effects, and alerts through separate controls.
- **30.3.4 Content ownership:** Track asset provenance and licenses alongside import/export settings so packaged content can be redistributed as intended.

### 30.4 Chapter checkpoint

- **30.4.1 Working result:** Polished silhouettes, animation, and sound improve recognition and operational feedback without obscuring the logistics/combat relationship.
- **30.4.2 Verification:** Recheck zoom legibility, color-independent cues, reduced-motion/audio settings, asset loading, GPU cleanup, and representative render performance.

## Chapter 31: Focused content, packaging, and release preparation

### 31.1 Add variety within the proven rules

- **31.1.1 Additional maps:** Vary deposits, bridge exposure, corridor length, defensible terrain, and sector positions while preserving viable alternatives.
- **31.1.2 Focused missions:** Explore sieges, evacuations, and long supply corridors through scenario goals rather than introducing many new economic subsystems.
- **31.1.3 Skirmish options:** Expose validated map, seed, difficulty, and supported match settings; keep tuning assumptions clear.
- **31.1.4 Mission regression:** Run scenario checks and tester sessions to ensure new geography still supports diagnosis, expansion, reserves, and recovery.

### 31.2 Make the game reliable outside the developer workspace

- **31.2.1 Product configuration:** Replace seed-facing names and startup defaults with Supply Front identity, release paths, and understandable user settings.
- **31.2.2 Portable packaging:** Stage required assets, definitions, licenses, and runtime dependencies and verify startup from a clean installation location.
- **31.2.3 User data:** Store settings, saves, replays, and logs in an appropriate writable user location and handle missing or unwritable paths clearly.
- **31.2.4 Supported-platform checks:** Build and exercise the platforms actually promised; document hardware requirements from measured results.

### 31.3 Prepare a reviewable release candidate

- **31.3.1 Player documentation:** Explain controls, the core loop, shortage diagnosis, save/replay behavior, and the supported modes without exposing internal architecture.
- **31.3.2 Quality checks:** Run core headless suites, representative match replays, tutorial paths, installation smoke checks, and accessibility checks.
- **31.3.3 Known-issue triage:** Prioritize crashes, corrupted state, unfair information, blocked progression, and unreadable failures before extra content.
- **31.3.4 Distribution scope:** Prepare the agreed PC release package and commercial content scope; publication and storefront actions remain separate explicit tasks.

### 31.4 Chapter checkpoint

- **31.4.1 Release result:** A clean installation launches a focused game with working tutorial/skirmish, readable feedback, persistence, settings, and documented support.
- **31.4.2 Verification:** Complete a release-candidate checklist with concrete evidence for packaging, supported platforms, regression matches, and remaining material issues.

## Chapter 32: Conditional extensions after the baseline

### 32.1 Explore shared operations only after ownership is clear

- **32.1.1 Cooperative supply rules:** Define shared inventory access, truck pools, depot service, claims, and permissions before enabling allied transfers.
- **32.1.2 2v2 experiments:** Test sector design, shared corridors, visibility, AI cooperation, and the measured larger-match performance budget.
- **32.1.3 Additional missions:** Expand operational scenarios when they add distinct decisions with the established roster and ten-cargo economy.
- **32.1.4 Additional factions:** Introduce asymmetry only after the shared roster is readable and balanced, with explicit effects on demand and counterplay.

### 32.2 Revisit transport and environmental extensions selectively

- **32.2.1 Rail justification:** Prototype rail only if truck throughput creates a useful strategic ceiling that bulk transport changes.
- **32.2.2 Rail scope if approved:** Start with warehouse terminals, setup costs, vulnerable endpoints, simple schedules, and conserved cargo before detailed signals or physics.
- **32.2.3 Aircraft and naval transport:** Add ports, ships, or aircraft only when maps and operational goals justify new transport and combat rules.
- **32.2.4 Other deferred systems:** Reconsider belts, weather, multiple fuels, and specialist ammunition individually; require a recurring player decision for each added burden.

### 32.3 Keep long-term scope evidence-based

- **32.3.1 Campaign persistence:** Design persistent progression and campaign state separately from ordinary match saves and focused missions.
- **32.3.2 Competitive services:** Consider ranking and broader online services only after network reliability, fairness, and match balance are established.
- **32.3.3 Cosmetic progression:** Evaluate optional cosmetics without adding purchased resource advantages or paid combat units to the baseline design.
- **32.3.4 New roadmap branches:** Give an approved extension its own concrete goals, prototype gate, and implementation todos rather than silently enlarging the baseline.

### 32.4 Chapter checkpoint

- **32.4.1 Decision result:** Each selected extension has a tested player benefit, a manageable technical cost, and an explicit scope decision.
- **32.4.2 Verification:** Unselected ideas remain deferred; approved experiments preserve conservation, visibility fairness, recovery, legibility, and acceptable player attention.
