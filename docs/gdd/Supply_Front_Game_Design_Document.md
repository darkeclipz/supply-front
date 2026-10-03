# Supply Front

**Game Design Document**

Version 0.1   •   3 October 2026   •   PC strategy game

### The game

Supply Front is a real time strategy game in which players build an industrial network and use it to sustain an army. Mines, processors, warehouses, factories, and frontline depots hold physical goods. Automated trucks move those goods along vulnerable routes. Territory matters because it contains resources, shortens delivery journeys, and controls access to the front.

The player commands both battles and the system that makes battles possible. A prepared offensive can exceed current production for several minutes; a successful raid can force an army to withdraw without destroying it. The design combines a short, readable production chain with operational warfare at the network level.

### Intended experience

The satisfying moment is seeing a network you designed support an attack you prepared: reserves leave a warehouse, a protected convoy reaches a forward depot, and the army pushes through. The equally important moment is understanding a failure and having enough time to respond.

| Baseline decision | Specification |
| --- | --- |
| Audience | RTS players who enjoy planning, economy building, and positional warfare |
| Platform and camera | PC; mouse and keyboard; overhead 3D camera with strategic zoom |
| Primary format | 1v1 skirmish; target match length 30 to 45 minutes |
| Setting | World War II era warfare in fictional industrial regions |
| Economy scale | Ten cargo types; short production chains; local inventories |
| Design status | Proposed rules and tuning values; validate through prototypes |

### How to use this document

Sections 1 to 10 specify the player experience and game rules. Sections 11 to 15 define presentation, implementation constraints, production scope, and validation. Numerical values are initial test values, not measured balance results.

## 1 Design pillars and boundaries

### Industry creates strategic choices

Factory location, transport capacity, inventory policy, and route redundancy must change what an army can accomplish. Additional production should sometimes be less useful than a shorter road, another truck, or a stocked forward depot. Every intermediate good needs a distinct military or logistical role.

### The network is readable

A stopped factory must show the missing item, where the item should come from, and why the delivery is failing. Players should identify a bottleneck from the map and one inspection panel. Production ratios exist, but individual belts, inserters, and vehicle schedules are abstracted.

### Supply creates windows for action

Units carry enough stock to survive a temporary cut. Prepared reserves allow intense attacks. A cut corridor produces warnings, degraded capability, and an opportunity to retreat before an army becomes immobile. Recovery should require decisions and exposure, without a long period of helpless waiting.

### Combat and logistics share attention fairly

Players issue intent through routes, reserve targets, unit stances, and production queues. Vehicles and units execute ordinary behavior automatically. Most logistics settings persist across a match; the player intervenes when geography, demand, or enemy pressure changes.

### Boundaries for the baseline game

- One shared faction roster first. Variety comes from maps, army composition, and economic choices.
- No individual ammunition calibers, food, worker housing, pollution, conveyor placement, or factory interior layouts.
- No transport orders for every truck. Direct control is available for emergency positioning, then the truck can return to its pool.
- No aircraft, naval combat, railways, or global market in the first playable version.
- Single player supports pause and speed controls. Competitive multiplayer runs continuously at a fixed speed.

### Player decisions at three scales

| Scale | Typical question | Expected cadence |
| --- | --- | --- |
| Tactical | Fight, withdraw, escort, or raid? | Seconds |
| Operational | Where should the next supply flow and reserve go? | One to three minutes |
| Industrial | What capacity and territory support the next army? | Several minutes |

## 2 Core loop and player agency

### The repeated loop

Scout resource deposits and corridors. Establish production near resources or defensible intersections. Connect industries to warehouses and factories. Build an army, accumulate its supplies, and establish a forward depot. Attack to gain territory or disrupt enemy capacity. Inspect the new demand and losses, then repair or expand the network.

Resources, combat units, and supplies reinforce each other: industry creates force; force secures corridors; corridors make additional industry usable. Warehouses separate production time from consumption time, letting the player choose when to concentrate that force.

### A typical decision sequence

- A tank factory is waiting for electronics. The player follows the highlighted inbound route and sees that wire delivery is delayed.
- A new smelter would not fix the problem: the existing copper smelter already has a full output buffer. The player assigns two more trucks and changes the route to a shorter bridge crossing.
- A scout then spots enemy raiders near that bridge. The player can escort the route, divert through a longer road, or stockpile supplies before risking a push.
- The army attacks while reserves are high. When the depot predicts two minutes of endurance, the player sends reinforcements, reduces artillery fire, or withdraws to a defensible position.

### Automation contract

The player selects destinations and priorities; the system chooses individual loads and eligible vehicles. Unit groups replenish automatically near compatible depots. Factories pull inputs for queued production. The system never changes a player locked route, spends reserved inventory on a lower priority request, or sends trucks through a known enemy controlled road without permission.

### What skill looks like

A strong player recognizes throughput limits, builds redundancy where it matters, times attacks around reserves, and chooses units appropriate to the supply corridor. Mechanical speed remains useful in combat, but a persistent logistics policy should not require repeated clicks to remain effective.

### What the player can infer

Visible trucks, warehouse activity, and loaded departures reveal economic activity under line of sight. Exact enemy inventory and factory queues remain hidden. This allows scouting to suggest an impending offensive without granting perfect information about its size.

## 3 Resource model and production

All cargo is counted in integer game units. One unit occupies one cargo slot, regardless of item type. Cargo units are an abstraction and do not represent literal tons, liters, or individual shells. Factories reserve a complete recipe locally before starting a cycle; outputs appear only when that cycle completes.

| Cargo | Source or recipe | Primary uses |
| --- | --- | --- |
| Iron ore | Iron mine | Metal |
| Copper ore | Copper mine | Wire |
| Crude oil | Oil pump | Fuel |
| Metal | Smelter: 2 iron ore → 1 metal | Construction, parts, ammunition |
| Wire | Copper smelter: 1 copper ore → 2 wire | Electronics |
| Parts | Machine shop: 2 metal → 1 part | Buildings, vehicles, repairs |
| Electronics | Electronics plant: 1 metal + 3 wire → 1 electronics | Advanced units, research |
| Fuel | Refinery: 1 crude oil → 2 fuel | Vehicles and diesel generators |
| Ammunition | Munitions plant: 1 metal → 4 ammunition | All weapon classes |
| Construction kits | Machine shop: 2 metal + 1 part → 1 kit | All buildings, road upgrades |

### Initial cycle values

Each mine or pump produces 60 raw units per minute. Metal smelting takes 2 seconds per recipe; wire smelting and refining take 2 seconds; parts and construction kits take 4 seconds; electronics take 5 seconds; ammunition takes 2 seconds. A machine shop runs one selected recipe at a time. Buildings can be duplicated to increase capacity.

### A readable electronics chain

One electronics plant produces 12 electronics per minute and consumes 12 metal plus 36 wire. A metal smelter produces 30 metal per minute. A wire smelter produces 60 wire per minute. These are maximum rates with sufficient inputs, power, output space, and transport; other consumers compete for the same goods.

For a dedicated electronics chain, the minimum raw supply is 24 iron ore and 18 copper ore per minute. Excess smelter capacity can feed parts or ammunition. The interface exposes both current output and these nameplate rates so a short input outage does not look like a change to the recipe.

### Buffers and physicality

Processors store 30 units of each input and 60 of each output. Vehicle factories store 60 of each recipe input and 120 each of fuel and ammunition for starting loads. Full output stops production. Reserved inputs stay within inventory until consumed at cycle start; reservations are claims, not extra cargo. Destruction loses production inventories and unfinished cycles. Goods move through transport, depot service, or defined salvage.

## 4 Construction power and progression

### Construction and the starting base

Each player starts with a field HQ, an engineer squad, one diesel generator, a warehouse, three trucks, and an infantry squad beside starter resource deposits. The warehouse contains 60 construction kits, 30 parts, 20 electronics, 120 fuel, and 160 ammunition. These reserves cover the first production chain; they are finite and physically stored. A starting fuel delivery connection from the warehouse to the generator lets the truck pool supply power immediately through the normal delivery rules; the generator starts empty, with no additional free fuel.

The opening goal is to establish oil extraction and refining before the fuel reserve runs out. The 120 starting fuel provides 20 minutes of operation for one generator if none is allocated to vehicles. Powering industry and fueling combat vehicles compete for the same stock.

Engineers place sites; a selected warehouse reserves and delivers their kits. Engineers then assemble buildings. The HQ can assemble nearby sites using an adjacent warehouse if no engineer survives. Canceling releases undelivered reservations; delivered goods become a crate.

| Structure | Kit cost | Build time | Function |
| --- | --- | --- | --- |
| Mine or pump | 4 | 20 seconds | Extracts a raw resource |
| Smelter or refinery | 6 | 30 seconds | Processes raw cargo |
| Machine shop or munitions plant | 8 | 35 seconds | Produces kits, parts, or ammunition |
| Electronics plant | 10 | 40 seconds | Produces electronics |
| Warehouse or forward depot | 6 / 4 | 25 / 20 seconds | Stores cargo or replenishes units |
| Vehicle factory | 12 | 45 seconds | Produces vehicles and trucks |
| Diesel generator | 5 | 25 seconds | Produces local power from fuel |
| Research laboratory | 8 | 30 seconds | Unlocks advanced equipment |

### Power

Power is local to connected cable networks. Mines, smelters, and refineries draw 1; other production and research buildings draw 2. Diesel generators supply 10 while running and consume 6 fuel per minute from their local inventory, which holds 60 fuel. Fuel must arrive by truck. An empty generator stops supplying power; players can switch generators off to conserve fuel. Storage, loading, infantry training, and depot service require no power. Players prioritize consumers; outages pause cycles without losing inputs. Oil pumps and refineries must receive generator power to establish a sustained fuel supply.

### Research and unit production

Infantry, scouts, engineers, and trucks are available immediately. A laboratory unlocks tanks and artillery for 12 electronics and 8 parts over 90 seconds. Inputs must reach the lab; unlocks apply faction wide. Unit recipes are consumed at production start. Cancellation returns 80 percent of each input, rounded down; overflow becomes crates. New units load their listed starting supplies before leaving.

### Recovery rules

Roads can be used immediately as engineer placed dirt paths; pavement costs kits. An HQ emergency workshop produces at most 6 kits and 6 parts per match without inputs, but only when those inventories and active production are empty. HQ infantry and engineer queues provide recovery tools. Losing all production does not create an endless free economy.

## 5 Transport and delivery rules

### Road graph and trucks

Trucks carry 40 cargo units. Dirt road speed is 8 map meters per second; paved speed is 12; off road speed is 4. Loading and unloading each take 5 seconds per trip. Ordinary road edges admit one truck per second in each direction; bridges admit one every two seconds total. Trucks pass through friendly units but queue at bays and constrained edges. Each warehouse has two bays; a depot and each producer have one.

Transport movement does not consume fuel in the baseline design. Fuel still travels physically to combat vehicles and generators. This keeps truck range strategically meaningful through time and risk while avoiding a circular failure in which fuel deliveries require fuel to restart.

### Player controls

The player connects a source to a destination, chooses permitted cargo, and sets minimum destination stock, target stock, priority, and an optional waypoint corridor. Transport requests appear automatically from production and stock policies. Warehouses share a truck pool; vehicles return to a safe source after completing or aborting a load.

### Dispatch algorithm

- Calculate the destination deficit after counting usable inventory and already reserved inbound cargo. This prevents duplicate deliveries.
- Use surplus above the source reserve floor. A shipment reserves cargo and one vehicle before loading; cargo leaves the source inventory when loaded.
- Serve priority classes in order: emergency defense, frontline supply, production, then reserve filling. Within a class, prefer the oldest eligible request, with journey time as a tie break.
- Batch allowed goods until the truck is full. Dispatch a partial load after 10 seconds of waiting, or immediately for an emergency request.
- Use a valid player corridor first. Otherwise choose the shortest permitted route. Each cargo type can move only from its declared source to its destination; automatic relays cannot form warehouse transfer loops.

### Disruption and control

Routes default to avoiding visible enemies and known enemy control. Fogged territory can be used, but the map labels its uncertainty. Players can explicitly allow a dangerous corridor. A blocked route first seeks a permitted alternative; if none exists, loaded trucks retreat to a reachable safe warehouse, keeping their manifest and releasing destination claims when rerouted.

A destroyed truck loses its cargo. A captured or destroyed destination releases its reservations. Queues expose the blocked edge or bay. Trucks never silently wait forever: after 15 seconds without progress, the request raises a grouped alert and the route panel presents available alternatives.

## 6 Warehouses and throughput planning

### Storage policy

A warehouse holds 2,000 cargo units; a forward depot holds 600 and accepts fuel, ammunition, and parts. Each player can set per item reserve floors and target stocks. Floors protect outbound inventory; targets request replenishment. The sum of targets cannot exceed capacity. Unallocated capacity can accept inbound goods, but delivery requests never exceed free space after reservations.

A lower floor can release goods during an emergency. Priorities select which requests receive scarce cargo and vehicles; they do not allocate a fixed percentage of every load. Factory inputs can draw directly from processors or through a declared warehouse. A second warehouse is useful only if its location, storage policy, or loading capacity improves the network.

### Worked transport example

A paved route spans 600 meters each way. At 12 meters per second, travel takes 100 seconds round trip. Loading and unloading add 10 seconds. A 40 unit truck therefore moves approximately 21.8 units per minute: 40 units every 110 seconds. Three trucks provide about 65.5 units per minute before queues or disruption.

An army demanding 60 supply units per minute leaves little margin. A 300 meter route with the same handling time completes a round trip in 60 seconds, so each truck moves 40 units per minute. This makes a forward warehouse or shorter crossing valuable without changing production recipes.

### Worked reserve example

A munitions network delivers 100 ammunition per minute to three depots, with 360 combined service capacity. An offensive spends 300 per minute. A shared 600 ammunition reserve lasts three minutes at a net draw of 200 per minute. A 6,000 reserve across warehouses lasts 30 minutes only if transport and depot service can deliver it. Stock at one depot cannot serve units outside its range; reserves do not fix inadequate throughput.

### The planning panel

| Display | Meaning |
| --- | --- |
| Production rate | Completed output per minute and maximum available rate |
| Delivery capacity | Expected cargo per minute on the chosen route |
| Consumption rate | Recent demand plus a selected army projection |
| Reserve endurance | Time until depletion at projected net draw |
| Critical constraint | The lowest capacity stage with a link to its map location |

Endurance is an estimate. The player can compare idle, moving, and intense combat scenarios. If delivery meets demand, the panel says stable rather than claiming an arbitrary infinite duration.

## 7 Combat supply and replenishment

### Local supplies

Combat units carry ammunition; powered combat vehicles also carry fuel. Ammunition is spent per weapon burst. Fuel is spent per second of movement, with no idle consumption. Units do not take damage simply because supplies run out. A single shared ammunition cargo feeds every weapon; different weapons consume it at different rates.

A depot transfers stored supply to friendly units within 60 meters at a total maximum of 120 cargo units per minute. It prioritizes manually selected groups, then units with the lowest supply percentage. Stock and service capacity are separate constraints. Trucks fill depot storage; they do not refill units directly.

### Supply states and unit behavior

| State | Trigger | Behavior |
| --- | --- | --- |
| Ready | More than 25 percent carried stock | Normal operation |
| Low | 25 percent or less of fuel or ammunition | Group warning; optional automatic return |
| Out of ammunition | No ammunition remaining | Weapons stop; movement and capture remain available |
| Out of fuel | No fuel remaining | Vehicle can crawl at 15 percent speed; weapons still fire |
| Replenishing | Inside depot range with available stock | Transfers supply subject to depot service capacity |

### Orders and stances

Groups support hold, cautious advance, assault, escort, and return to depot. The default supply stance warns while following orders. An optional sustain stance returns at 25 percent stock to a reachable depot, avoiding known enemy controlled edges. Returning to the previous assignment requires the player to enable resume after refill.

An empty vehicle uses an emergency drive for a slow retreat. While crawling it cannot dash, tow, or gain road speed. Attackers can overtake the retreating force; fuel exhaustion does not leave permanent abandoned obstacles.

### Combat rules

Use conventional line of sight, directional armor, terrain cover, and projectile travel. Infantry in cover counters raiders; tanks break exposed positions; artillery pressures static defenses but consumes supply quickly. Weapon range, accuracy, armor, and health require combat prototype tuning before firm values are assigned.

Units automatically attack visible targets within their stance. The baseline has no repeatable activated abilities. Indirect fire needs a spotter or targets a selected location at reduced accuracy. Supply disruption gradually reduces capacity, giving players time to respond.

## 8 Unit roster and supply budgets

Costs below consume local parts and electronics. Fuel and ammunition are additional starting loads transferred into the unit. Times assume one production slot. Rates describe sustained movement or sustained firing; ordinary battles spend less. Infantry is controlled as a five soldier squad with one shared inventory.

| Unit | Build cost and time | Capacity F / A | Use F / A per minute |
| --- | --- | --- | --- |
| Infantry squad | 2 parts; 15 seconds | 0 / 20 | 0 / 4 |
| Engineer squad | 2 parts; 20 seconds | 0 / 10 | 0 / 2 |
| Scout car | 3 parts + 1 electronics; 20 seconds | 20 / 12 | 1 / 3 |
| Tank | 10 parts + 3 electronics; 45 seconds | 40 / 30 | 2 / 6 |
| Artillery vehicle | 8 parts + 4 electronics; 50 seconds | 25 / 40 | 1.5 / 12 |
| Supply truck | 2 parts; 15 seconds | 40 cargo slots | No operating supply use |

F means fuel and A means ammunition. Carrying capacity gives about 20 minutes of uninterrupted tank movement and five minutes of uninterrupted tank firing. An artillery vehicle holds approximately 3.3 minutes of sustained fire. Capacities are tuning levers for raid impact and player reaction time.

### Roles and dependencies

Infantry trains at the HQ, captures structures, defends roads, and uses little transport capacity. Engineers construct and repair; repairs consume one part per 10 percent of maximum health restored, supplied from carried cargo or a nearby depot. An engineer can carry 10 parts in addition to ammunition. Scout cars reveal corridors and hunt vulnerable trucks. They lose direct fights against prepared defenders.

Tanks need electronics and a reliable fuel corridor, but provide mobile breakthrough power. Artillery spends ammunition quickly and benefits from prepositioned reserves. Supply trucks are civilian logistics units without weapons; they belong to the transport pool and can be escorted using a combat group order.

### Example force

Eight tanks, two artillery vehicles, and six infantry squads consume up to 96 ammunition per minute while firing continuously. The vehicles consume up to 19 fuel per minute while moving continuously. Their combined worst case is 115 cargo per minute, close to one depot’s 120 unit service limit. A player may need two depots to spread units out, accommodate repairs, or survive a raid.

### Composition as an economic choice

Infantry can secure a long or damaged corridor with modest supply. A tank force imposes a high upfront parts burden and a steady fuel burden. Artillery exchanges safe range for ammunition throughput. Counterplay should arise from both combat matchups and the supply capacity supporting them.

## 9 Map control objectives and capture

### Map structure

The reference 1v1 map is roughly 2 by 2 kilometers. Each home area has modest deposits of all three raw resources. Larger contested deposits sit near outer lanes. Three victory sectors lie across the center. At least two viable ground corridors connect each home area to the contested region, with one shorter exposed path and one longer defensible route.

Deposits are finite, but starter deposits should support at least 20 minutes of typical use before depletion. The interface warns at 25 percent remaining. Exact reserves are map tuning values. Geography should reward expansion without forcing an early all or nothing resource race.

### Victory

After a five minute opening period, controlling at least two of the three sectors grants one victory point per second. A player wins at 900 points or by destroying the enemy HQ. Uncontested infantry captures a sector in 20 seconds. Vehicles alone cannot capture. Points do not decrease, and a match can end before either industrial base is destroyed.

This creates a reason to advance. Cutting supply supports the objective fight; destroying trucks is not a separate win condition. A player may concede a sector to rebuild, but must recover before the enemy’s points reach the target. The score and estimated time to victory remain visible.

### Warehouse capture

Infantry can capture an enemy warehouse or depot after 30 uninterrupted seconds beside it, provided no defending combat unit is within 60 meters. Capture transfers ownership and surviving inventory. Routes and source reservations are reset; the captor must connect the building to a friendly network. Exact contents become visible after capture.

Production buildings and HQs cannot be captured in the baseline. Destruction of storage drops 25 percent of its inventory into visible salvage crates; the rest is lost. Engineers collect crates through ordinary transport requests. The former owner can recapture intact storage, creating counterattack opportunities.

### Counterplay and recovery

Bridges have high health and clear under attack warnings. A destroyed bridge blocks its graph edge; engineers rebuild using 4 kits in 45 seconds after delivery. The map must retain an alternate route. Road damage slows travel before fully blocking it. Deliberate demolition takes 20 seconds and is canceled by incoming damage, making retreat under pressure a real choice.

## 10 Match pacing and teaching scenario

| Phase | Player goal | Pressure |
| --- | --- | --- |
| Opening 0 to 5 minutes | Build metal, parts, kits, ammunition, and first routes | Scout approaches; small raids can disrupt exposed growth |
| Expansion 5 to 12 minutes | Contest sectors and establish a forward depot | Balance tech spending against road defense |
| Offensives 12 to 25 minutes | Use tanks or artillery with prepared reserves | Corridors, service limits, and counterattacks matter |
| Resolution 25 to 45 minutes | Secure points or break the industrial base | Depleted deposits and longer delivery routes change plans |

These windows describe the target rhythm, not locked phases. Players may choose early infantry pressure, rapid industrial growth, or a defensive stockpile. Tanks should arrive through a visible investment that creates scouting and counterplay opportunities.

### Tutorial mission The River Crossing

The player inherits a working iron and copper chain beside a river. The opening task is to connect the electronics plant and produce a scout car. A route overlay identifies the wire deficit and lets the player assign a source and trucks. There is no combat until the first delivery succeeds.

Next, the player fills a forward depot with fuel and ammunition and takes a neutral sector using infantry and two tanks. The mission introduces the army endurance panel before asking the player to fight. It shows the difference between factory output, truck delivery, and depot service.

An enemy raid then closes the short bridge route. The army keeps fighting from carried stock. A warning highlights the interrupted corridor and presents three valid responses: escort engineers to repair the bridge, redirect trucks along the longer southern road, or pull the army back to the home depot.

The final task is to prepare a two minute offensive and capture an enemy depot. Its inventory supports the last push if the player reconnects it. Success is taking the sector, not maximizing factory output. Optional objectives reward keeping trucks alive and solving the interruption without losing a combat group.

### Failure and learning

Restart points occur before the bridge raid and final offensive. Single player can pause while the player inspects a bottleneck. The debrief pairs the army’s supply history with the relevant route interruption and shows one concrete alternative. It should explain causality without pretending every defeat has one cause.

### Modes after the prototype

Expand from 1v1 to cooperative operations and 2v2 after shared supply ownership rules are tested. A campaign can explore sieges, evacuations, and long corridors. Competitive ranking and additional factions follow only after the baseline game is readable and balanced.

## 11 Interface and information design

### Normal battlefield view

The main view emphasizes armies, depots, and terrain. A top bar shows victory score and a compact overview of cargo totals, with stored, reserved, and in transit values separated. Global totals are informational; spending always uses a local eligible inventory. Selecting a group shows fuel, ammunition, reachable depots, and estimated endurance.

### Logistics overlay

A toggle displays sources, destinations, route direction, cargo mix, and current flow against available capacity. Thickness represents delivery volume. Solid lines are active; dashed lines are awaiting cargo; a blocked symbol marks interruption. Color has a redundant icon and text label. Strategic zoom aggregates trucks into route flow indicators without hiding raids or losses.

### Warehouse and factory panels

- Warehouse panel: inventory, available space, reserve floors, targets, inbound claims, truck pool, and loading bay queues.
- Factory panel: recipe, queue, local inventory, completed output, expected completion, and the current constraint.
- Route panel: source, destination, permitted cargo, vehicle count, journey time, throughput estimate, priority, and corridor policy.
- Army panel: selected composition, carried stock, projected demand, replenishment access, and low supply stance.

### Example diagnostic

Tank factory waiting for 2 electronics. Electronics plant has output ready. Route North Bridge is blocked. Two loaded trucks are returning to Warehouse Alpha. Southern Road adds 45 seconds. The player can inspect the route, approve the detour, or change the tank queue from this panel.

### Alerts

Alerts group related failures by root cause. A destroyed bridge should produce one corridor warning with affected destinations, rather than a separate message for every stopped factory. Warnings have persistent map markers and a time since last successful delivery. Routine shortages remain in panels; imminent army depletion, attacks, and invalid routes receive higher prominence.

### Controls and accessibility

Support control groups, shift queued orders, contextual right click commands, drag route connections, copyable warehouse policies, and fully remappable keys. Offer scalable text, color independent status cues, reduced camera motion, high contrast selection, and adjustable alert audio. Single player provides pause, three speed settings, and optional tutorial guidance.

The minimum first use test is whether a new player can explain why a factory stopped and choose a corrective action in under 30 seconds, using only the game interface.

## 12 Visual and audio direction

### Readable industrial warfare

Use grounded, stylized 3D with compact building silhouettes and distinct production identities. Iron facilities show bulky dark ore hoppers, copper processing shows visible wire spools, oil facilities use tanks and pipes, and electronics plants use enclosed assembly buildings. Cargo colors support recognition but never carry essential information alone.

Factories need obvious input and output docks so truck behavior appears purposeful. Warehouses show activity through doors, loading bays, and stacked cargo. Visible details convey operation without representing every internal machine. At distant zoom levels, clear icons replace small industrial animation.

### Battlefield hierarchy

Friendly and enemy outlines must remain readable against terrain. Combat vehicles are larger and more visually prominent than trucks. Depots are easy to identify from operational zoom because they govern endurance. Smoke and destruction communicate state changes without permanently covering roads, selection outlines, or unit supply indicators.

### Audio as operational feedback

Use distinct sounds for a complete production cycle, convoy arrival, depot replenishment, and route interruption. Ambient industry fades with camera distance. Combat alerts take priority over repeated factory events. A low supply group speaks once per state transition, with a cooldown and an option to suppress repeated warnings.

### Setting and tone

The game uses a World War II era setting, with rival forces fighting over fictional industrial regions. Units, buildings, and equipment use period-inspired guns, armor, trucks, field headquarters, and diesel generators. Electronics cargo represents radios, wiring assemblies, and electrical control equipment. Early content avoids named real nations and recreations of specific historical battles. Narrative missions frame bridges, depots, and resource fields as tangible goals rather than treating them as background scenery.

### Feedback requirements

| Event | Required visual feedback | Required information |
| --- | --- | --- |
| Production stopped | Building state icon | Missing input, power, or output space |
| Truck route broken | Route marker and diverted truck | Blocked location and affected destination |
| Unit supply low | Group badge and inventory bar | Resource, endurance estimate, depot access |
| Warehouse captured | Ownership change and reset routes | Inventory acquired and connection required |

Prototype assets may be simple shapes, but silhouettes, state icons, and information hierarchy must already support these requirements. Art polish cannot be used to postpone testing whether the systems are understandable.

## 13 AI and simulation requirements

### Opponent behavior

The AI builds a viable metal, kit, parts, and ammunition chain before expanding. It plans an army composition, projects that army’s demand, and assigns transport capacity and reserve targets. It advances only when a reachable depot can support the plan, with exceptions for urgent sector defense or a short raid.

Operational decisions compare sector value, observed enemies, route exposure, travel time, and available reserves. The AI can defend trucks, raid an exposed corridor, use a longer route, or withdraw a low supply army. Economic and combat planners must share the same inventory and transport predictions used by the player interface.

Baseline difficulty changes planning depth, reaction delay, and risk tolerance. The AI follows player visibility rules and receives no hidden inventory knowledge. Debug scenarios may use revealed information, but that mode is explicitly separate from playable difficulty. Resource bonuses are optional later and must be disclosed.

### Simulation model

Store inventories, recipe reservations, manifests, route claims, and truck assignments as authoritative state. Use integer cargo and fixed point movement and fuel accumulation to preserve repeatability. Proposed fixed simulation rate is 10 updates per second; logistics dispatch runs once per second or when a major route event occurs.

Vehicles use a road graph for strategic routing and lightweight local motion for visual spacing. Graph edits invalidate affected cached paths. Combat units need separate terrain navigation. The renderer can aggregate distant traffic, but every truck’s cargo and vulnerability remain simulated.

### Proposed capacity targets

Prototype stress target: 200 combat entities, 150 trucks, and 100 industrial or storage buildings across both players. Full game target: 600 combat entities and 300 trucks on a 2v2 map. These are budgets to test, not performance claims. Measure simulation time separately from graphics on a declared reference PC; set hardware requirements after profiling.

### Persistence and multiplayer

Save and replay data include orders, random seeds, inventory reservations, route edits, and AI choices. A save loaded during an interrupted delivery must preserve its manifest and claims. Establish authoritative multiplayer state and replay checks during the vertical slice. Deterministic lockstep versus server synchronization is an engineering decision after profiling, not a prerequisite for the first offline prototype.

## 14 Prototype and production scope

### First playable prototype

Build an offline 1v1 graybox with a scripted opponent, one map, the ten cargo types, road graph transport, warehouse policies, one forward depot, and infantry, scout cars, tanks, artillery, engineers, and trucks. Include the minimum HQ, research, power, construction, and recovery rules needed to make the economy playable end to end.

The prototype must support three complete situations: starting a production chain from finite reserves, sustaining an attack from a forward depot, and recovering after a corridor raid. A sandbox can spawn enemies or close a bridge for repeatable tests, but the player economy follows ordinary rules.

| Milestone | Deliverable | Exit condition |
| --- | --- | --- |
| Economy sandbox | Recipes, inventories, construction, routes, diagnostics | No duplicated or lost goods outside explicit sinks; common shortages explained |
| Combat prototype | Roster, supply use, depots, movement, sectors | An offensive needs planning; a supply cut permits at least one viable response |
| Integrated playable | One map and scripted enemy; complete victory loop | New testers finish matches and identify their main bottlenecks |
| Vertical slice | Opponent planner, tutorial, save and replay, multiplayer trial | Core loop remains readable under combat and transport load |
| Content expansion | More maps and mission types; 2v2 testing | Map and roster variety add choices without excessive logistics work |

### Explicitly deferred

Railways, ports, ships, aircraft, additional factions, conveyor details, weather, multiple fuel types, specialist ammunition, campaign persistence, ranking, and cosmetic progression. Rail can later provide high throughput between warehouses with substantial setup and vulnerable terminals. It is justified only if trucks create an interesting capacity ceiling that rail changes.

### Dependencies and staffing

The critical work spans systems design, simulation and navigation engineering, combat and interface engineering, technical art, audio, and QA. Headcount and schedule require an engine choice, team availability, and measured prototype performance. Do not attach a production budget until the routing, attention, and multiplayer risks have been tested.

### Release proposition

A premium PC release is the baseline business assumption. There are no purchased resource advantages or paid combat units in this design. The first commercial scope should favor strong skirmish play and a focused set of missions over a large faction roster.

## 15 Validation risks and open decisions

### Playtest questions and success criteria

Run early tests with both experienced RTS players and players comfortable with factory games. Start with observation and short debriefs; use telemetry to explain observed problems. The thresholds below are proposed acceptance gates for the integrated prototype.

| Question | Proposed gate |
| --- | --- |
| Can players diagnose failures? | At least 80 percent of testers resolve a seeded input or route shortage within 30 seconds after the tutorial |
| Does automation hold up? | During stable operations, median logistics attention stays below 35 percent of active interaction time |
| Are raids recoverable? | A seeded corridor cut gives at least 60 seconds of effective local fighting stock and one reachable fallback |
| Do reserves matter? | Testers can sustain a short high consumption attack that current production alone cannot support |
| Is expansion worthwhile? | Players choose contested capacity or a shorter corridor in most matches rather than indefinitely stacking safe factories |
| Is the economy conserved? | Stored and carried cargo reconcile with explicit sources and sinks after all state changes; reservation claims never count as extra goods |

### Primary risks and responses

Attention overload: simplify repeated settings, improve diagnostics, and lengthen supply buffers before removing physical goods. Passive turtling: tune sector pressure and contested deposits before adding more destruction incentives. Raid snowballing: lengthen local reserves, lower rebuild costs, and preserve alternate paths. Traffic frustration: expose queues and limit road congestion to understandable graph capacities.

Dominant stockpiling: pressure storage through capacity and geography, and keep armies expensive enough that inventory competes with force. Logistics becoming irrelevant: shorten reserve endurance or increase operational demand after verifying that routes and alerts remain readable. Recipe clutter: remove intermediates that do not create a recurring placement, capacity, or composition decision.

### Decisions to resolve through the prototype

Determine the best reserve endurance under real combat, whether power adds enough choice to justify its attention cost, and how often direct processor to factory routes should replace warehouses. Tune capture time, salvage share, road capacity, and objective scoring from match observations. Confirm whether emergency vehicle crawl preserves recovery without making fuel shortages trivial.

### Commitment gate

Proceed to a larger production only when testers can fight while their supply network runs, explain the consequences of a raid, and make useful changes to their economy. The core product is the interaction between those decisions. More content should follow evidence that the interaction is enjoyable and legible.
