IMPORTANT: THIS DOCUMENT CONTAINS THE DESIGN OF A VERY SIMILAR GAME, BUT WE WANT TO USE THIS SET UP AS INSPIRATION ONLY!

---

That combination changes the architecture quite a bit—in a good way. You’re essentially building three games that share one simulation:

**Command & Conquer:** selection → orders → movement → combat → bases.  
**Factorio-lite:** resources → inventories → recipes → production chains.  
**Transport/logistics game:** warehouses → transport requests → trucks → stations → trains.

The best approach is to make those three layers share a common **entity/component simulation**, rather than bolting logistics onto an RTS afterward.

I’d use **C++20 + raylib + CMake**, with **EnTT** once the basic prototype works. EnTT is a modern header-only C++ ECS library whose current documentation is built around entities, components, registries, views, and systems; that model fits this project especially well. [GitHub](https://github.com/skypjack/entt/wiki/Entity-Component-System?utm_source=chatgpt.com)

I would probably use ordinary raylib directly from C++ rather than wrapping everything in `raylib-cpp`. raylib's C API is already very C++ friendly. `raylib-cpp` exists and is a reasonable optional wrapper if you prefer RAII-style objects. [GitHub](https://github.com/RobLoach/raylib-cpp?utm_source=chatgpt.com)

---

# 1. Project foundation

Before making an RTS, get a small C++ project running with:

```text
C++20
CMake
raylib
EnTT
nlohmann/json
Catch2
```

Don't add networking, physics engines, scripting languages, ImGui, audio middleware, etc. yet.

The official CMake tutorial is good enough to learn the small amount of CMake you'll actually need. [CMake](https://cmake.org/cmake/help/latest/guide/tutorial/index.html?utm_source=chatgpt.com)

[CMake tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html?utm_source=chatgpt.com)

For raylib, work through the official introductory course, particularly input, textures, collisions and the game loop. [GitHub](https://github.com/raysan5/raylib-intro-course?utm_source=chatgpt.com)

[Official raylib introductory course](https://github.com/raysan5/raylib-intro-course?utm_source=chatgpt.com)

Also bookmark the raylib example collection. It currently contains more than 200 examples and is much more useful than trying to memorize the API. [GitHub](https://github.com/raysan5/raylib/blob/master/examples/README.md?utm_source=chatgpt.com)

[Official raylib examples](https://github.com/raysan5/raylib/tree/master/examples?utm_source=chatgpt.com)

### First program

Do **not** make a game yet.

Make:

```text
window
 ├─ grid
 ├─ movable camera
 ├─ mouse world coordinates
 ├─ FPS counter
 └─ debug panel
```

You should understand the difference between:

```cpp
Vector2 mouseScreen;
Vector2 mouseWorld;
```

before proceeding.

The raylib-extras repositories are particularly useful here because they have camera/world-space examples, mouse-centered zoom, camera clamping and C++ examples. [GitHub](https://github.com/raylib-extras/examples-cpp?utm_source=chatgpt.com)

[raylib-extras C++ examples](https://github.com/raylib-extras/examples-cpp?utm_source=chatgpt.com)

---

# 2. Build the simulation clock

Do this unusually early.

Your renderer might run at:

```text
73 FPS
144 FPS
240 FPS
```

but your actual game should simulate at something like:

```text
20 ticks/sec
```

or:

```text
30 ticks/sec
```

You can interpolate unit positions visually between ticks.

Read **Fix Your Timestep!**. It explains why simulation should not depend directly on rendering frame time, including the accumulator approach and interpolation. [Gaffer On Games](https://gafferongames.com/post/fix_your_timestep/)

[Fix Your Timestep! — Glenn Fiedler](https://gafferongames.com/post/fix_your_timestep/?utm_source=chatgpt.com)

Conceptually:

```cpp
while (!WindowShouldClose())
{
    processInput();

    accumulator += GetFrameTime();

    while (accumulator >= SIM_DT)
    {
        simulation.tick(SIM_DT);
        accumulator -= SIM_DT;
    }

    float alpha = accumulator / SIM_DT;

    renderer.draw(simulation, alpha);
}
```

Keep this separation forever.

This gives you a foundation for:

- deterministic-ish gameplay,
- production timers,
- unit cooldowns,
- truck scheduling,
- train reservations,
- pausing,
- fast-forward,
- replays,
- debugging,
- eventual multiplayer.

---

# 3. RTS camera + selection

Now build your first actual C&C interaction.

Implement:

```text
WASD camera
edge scrolling
mouse-wheel zoom
left-click selection
drag selection box
Shift+click
Ctrl groups 1–9
right-click command
```

Don't create combat yet.

Put 50 rectangles/tanks on screen and make selection work.

Raylib's official mouse/input examples cover the input primitives you need. [GitHub](https://github.com/raysan5/raylib-intro-course/blob/main/README.md?utm_source=chatgpt.com)

### Important architecture

The input layer must **not move units directly**.

Wrong:

```cpp
if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    unit.position = mouseWorld;
```

Correct:

```text
Mouse
 ↓
SelectionController
 ↓
PlayerCommand
 ↓
Simulation command queue
 ↓
CommandSystem
 ↓
Unit orders
```

For example:

```cpp
struct MoveCommand
{
    PlayerId player;
    std::vector<EntityId> units;
    Vector2 destination;
};
```

That distinction becomes incredibly useful later.

It also maps closely to the Command pattern from *Game Programming Patterns*. That book's Component, Event Queue, Update Method and Spatial Partition chapters are all worth reading for this project. [Game Programming Patterns](https://gameprogrammingpatterns.com/event-queue.html?utm_source=chatgpt.com)

[Game Programming Patterns](https://gameprogrammingpatterns.com/?utm_source=chatgpt.com)

---

# 4. Introduce the ECS

Now switch your prototype entities to EnTT.

Your entity itself contains **nothing**:

```cpp
entt::entity tank = registry.create();
```

You attach capabilities:

```cpp
registry.emplace<Transform>(tank);
registry.emplace<Selectable>(tank);
registry.emplace<Team>(tank);
registry.emplace<Health>(tank);
registry.emplace<GroundMover>(tank);
registry.emplace<Weapon>(tank);
```

A warehouse might be:

```text
Transform
Building
Team
Health
Inventory
StorageProvider
StorageRequester
```

A truck:

```text
Transform
GroundMover
Team
Health
Inventory
Hauler
CommandQueue
```

A refinery:

```text
Transform
Building
Team
Health
Inventory
Crafter
```

Notice that **Truck does not inherit Unit** and **Refinery does not inherit Building**.

They're combinations of data.

Read the EnTT ECS crash course at this point. [GitHub](https://github.com/skypjack/entt/wiki/Entity-Component-System?utm_source=chatgpt.com)

[EnTT ECS crash course](https://skypjack.github.io/entt/md_docs_2md_2entity.html?utm_source=chatgpt.com)

---

# 5. World grid + building placement

Your world should have a logical grid even if units move continuously.

Something like:

```cpp
struct Tile
{
    TerrainType terrain;
    bool buildable;
    bool blocked;
    float movementCost;
};
```

Maintain separate concepts for:

```text
terrain grid
construction occupancy
navigation grid
resource deposits
rail graph
```

Do not make the graphics map authoritative.

Implement:

```text
building ghost
green/red placement
grid snapping
footprints
occupancy
rotation
construction confirmation
```

Example:

```text
Power Plant
XXXX
XXXX
XXX.
```

can occupy a different footprint than its image.

---

# 6. A* pathfinding

Now right-click needs to mean:

> Find a route there.

Read Red Blob Games' A* material. It's probably the best interactive pathfinding explanation available and includes implementation guidance. [Red Blob Games](https://www.redblobgames.com/pathfinding/a-star/implementation.html?utm_source=chatgpt.com)

[Red Blob Games — A* implementation](https://www.redblobgames.com/pathfinding/a-star/implementation.html?utm_source=chatgpt.com)

Start with:

```text
8-direction grid
A*
binary priority queue
walkability
movement costs
```

Do **not** immediately implement:

```text
navmeshes
HPA*
JPS
flow fields
dynamic hierarchical navigation
```

Get basic A* working first.

### Critical RTS change

Don't run:

```text
500 units × A*
```

every tick.

Instead implement a:

```cpp
class PathService
{
public:
    PathRequestId request(EntityId entity,
                          GridPos start,
                          GridPos goal);

    void update(PathBudget budget);
};
```

Path requests become queued jobs.

Factorio's developers have written interesting material about exactly this problem: large numbers of pathfinding requests can overwhelm the pathfinder, and Factorio spreads path work across simulation ticks. [Factorio](https://direct.factorio.com/blog/post/fff-317?utm_source=chatgpt.com)

[Factorio Friday Facts #317 — pathfinding](https://factorio.com/blog/post/fff-317?utm_source=chatgpt.com)

---

# 7. Multiple-unit movement

This is where your prototype starts feeling like C&C.

Selecting 40 tanks and right-clicking should not result in:

```text
40 tanks all aiming for exactly (500, 300)
```

Generate formation slots around the destination:

```text
X X X X X
 X X X X
X X X X X
```

and assign units to slots.

Initially, nearest-unit → nearest-slot is adequate.

Later you can improve the matching algorithm.

Add local separation so units avoid occupying the same space.

At this point also implement a simple **spatial hash/grid**.

Instead of checking every unit against every unit:

```text
O(n²)
```

store units in spatial cells and query nearby cells.

The Spatial Partition chapter in *Game Programming Patterns* specifically demonstrates a fixed-grid approach for efficiently finding nearby units. [Game Programming Patterns](https://gameprogrammingpatterns.com/spatial-partition.html?utm_source=chatgpt.com)

Your structure might look like:

```cpp
class SpatialGrid
{
public:
    void insert(EntityId, Vector2 position);
    void move(EntityId, Vector2 oldPos, Vector2 newPos);

    std::span<EntityId> query(Rectangle);
    std::span<EntityId> nearby(Vector2, float radius);
};
```

You'll reuse it for:

```text
collision
target acquisition
selection
area damage
fog of war
AI queries
```

---

# 8. Orders and state machines

Now introduce unit orders.

```cpp
using UnitOrder = std::variant<
    MoveOrder,
    AttackOrder,
    AttackMoveOrder,
    GuardOrder,
    HarvestOrder,
    DeliverOrder
>;
```

Each controllable unit gets:

```cpp
struct OrderQueue
{
    std::deque<UnitOrder> orders;
};
```

Shift-right-click appends.

Normal right-click replaces.

This is important because eventually a truck may have:

```text
DriveToWarehouse
PickUp
DriveToFactory
Unload
Return
```

while a tank may have:

```text
AttackMove
AttackEnemy
ReturnToFormation
```

Same scheduling idea; completely different components process them.

---

# 9. C&C-style combat

Now implement the fun bit.

Use components such as:

```cpp
struct Health
{
    int hp;
    int maxHp;
};

struct Weapon
{
    WeaponDefId definition;
    SimTime cooldownRemaining;
};

struct Target
{
    EntityId entity;
};
```

and systems:

```text
TargetAcquisitionSystem
WeaponSystem
ProjectileSystem
DamageSystem
DeathSystem
```

Do not write:

```cpp
Tank::attack()
```

Instead:

```cpp
WeaponSystem::update(registry);
```

operates on entities having:

```text
Transform
Weapon
Team
Target
```

### First combat milestone

Only implement:

```text
tank
rifle infantry
turret
```

with:

```text
hitscan rifle
projectile cannon
```

Ignore armor types initially.

Then add:

```text
damage type
armor type
range
reload
accuracy
splash radius
```

---

# 10. Buildings + production

Now add classic C&C production.

Components:

```cpp
struct ConstructionSite
{
    BuildingDefId building;
    float progress;
};

struct ProductionQueue
{
    std::deque<ProductionOrder> jobs;
};
```

Separate:

```text
construction
```

from:

```text
manufacturing
```

because they become radically different once crafting exists.

For example:

```text
Barracks
    produces soldiers

Factory
    produces vehicles

Electronics Plant
    crafts components

Warehouse
    stores resources
```

---

# 11. Make everything data-driven

At this stage, stop hard-coding units.

Instead of:

```cpp
Tank tank;
tank.hp = 800;
tank.speed = 2.5;
```

load:

```json
{
  "id": "medium_tank",
  "health": 800,
  "movementSpeed": 2.5,
  "weapon": "120mm_cannon",
  "cost": {
    "steel": 20,
    "electronics": 4
  }
}
```

Use static definition objects:

```cpp
struct UnitDefinition;
struct BuildingDefinition;
struct ItemDefinition;
struct RecipeDefinition;
struct WeaponDefinition;
```

owned by:

```cpp
class DefinitionDatabase;
```

`nlohmann/json` is a straightforward C++ choice and has current documentation covering parsing, serialization and arbitrary type conversion. [Nlohmann](https://nlohmann.github.io/json/?utm_source=chatgpt.com)

[JSON for Modern C++ documentation](https://nlohmann.github.io/json/?utm_source=chatgpt.com)

This means balancing your game later becomes:

```text
edit JSON
reload
test
```

instead of:

```text
change C++
compile
run
```

---

# 12. Inventory system

This is where the Factorio portion starts.

Everything that stores goods receives:

```cpp
struct Inventory
{
    std::vector<ItemStack> slots;
};
```

where:

```cpp
struct ItemStack
{
    ItemId item;
    int quantity;
};
```

Use the exact same inventory model for:

```text
warehouse
factory
truck
train wagon
mine
construction site
```

That commonality becomes enormously valuable.

---

# 13. Recipe system

Recipes should also just be data.

```cpp
struct Ingredient
{
    ItemId item;
    int amount;
};

struct RecipeDefinition
{
    RecipeId id;

    std::vector<Ingredient> inputs;
    std::vector<Ingredient> outputs;

    SimTime duration;
};
```

Example:

```text
Copper Ore
    ↓
Copper Plate
    ↓
Copper Wire ─┐
             ├→ Electronic Component
Iron Plate ──┘
```

That's approximately the complexity target you've described.

Factorio's green/electronic circuit is a useful reference point: it is an early intermediate requiring copper cable plus iron plate and subsequently feeds many downstream production recipes. [Official Factorio Wiki](https://wiki.factorio.com/Electronic_circuit?utm_source=chatgpt.com)

[Factorio electronic circuit reference](https://wiki.factorio.com/Electronic_circuit?utm_source=chatgpt.com)

I would intentionally cap your initial production graph around:

```text
Raw:
Iron ore
Copper ore
Coal

Processed:
Iron plate
Copper plate

Intermediate:
Wire
Gears
Electronics

Finished:
Ammo
Vehicle parts
Tank
Truck
```

That's already enough to create interesting logistics.

---

# 14. Crafting machines

Machines get:

```cpp
struct Crafter
{
    RecipeId recipe;

    Inventory input;
    Inventory output;

    SimTime progress;
    bool running;
};
```

Your `CraftingSystem` basically does:

```text
Has ingredients?
   ↓ yes
consume ingredients
   ↓
advance timer
   ↓
produce output
```

Start with manually selected recipes.

No smart production network yet.

---

# 15. Warehouse system

Now comes the part that differentiates your game.

A warehouse isn't simply:

```text
big inventory
```

It acts as a logistics endpoint.

Give it:

```cpp
struct LogisticsEndpoint
{
    LogisticsMode mode;
};
```

Modes might include:

```text
Store
Provide
Request
ProvideAndRequest
```

And configurable policies:

```text
Keep at least 100 iron
Keep at most 500 iron

Request electronics if < 30
Provide electronics if > 100
```

That gives you the beginnings of an economy without copying Factorio belts.

---

# 16. Logistics requests

This system should not involve trucks yet.

Implement a pure planning layer first.

For example:

```cpp
struct LogisticsRequest
{
    EntityId destination;
    ItemId item;
    int quantity;
    int priority;
};
```

Warehouse A:

```text
Iron: 800
desired maximum: 500

→ offers 300
```

Factory B:

```text
Iron: 5
desired minimum: 100

→ requests 95
```

Then:

```cpp
LogisticsPlanner
```

matches:

```text
95 iron
Warehouse A → Factory B
```

into:

```cpp
TransportJob
```

This separation is extremely important:

```text
Demand
↓
Planning
↓
Transport job
↓
Vehicle execution
```

Don't make the truck decide what society needs.

---

# 17. Reservations

This deserves its own phase because logistics games break without it.

Suppose two trucks both see:

```text
Warehouse:
50 electronics
```

and both decide to take 50.

You've accidentally created 100 electronics.

So introduce:

```cpp
struct Reservation
{
    EntityId owner;
    ItemId item;
    int amount;
};
```

Inventory effectively contains:

```text
physical quantity
reserved quantity
available quantity
```

and transport jobs reserve:

```text
source goods
destination capacity
```

before a truck starts moving.

You'll reuse the same concept for trains later.

---

# 18. Trucks

A truck now becomes surprisingly simple:

```text
Transform
GroundMover
Inventory
Hauler
OrderQueue
```

Its logistics state machine:

```text
Idle
 ↓
AssignedJob
 ↓
DriveToPickup
 ↓
Loading
 ↓
DriveToDestination
 ↓
Unloading
 ↓
Idle
```

Something like:

```cpp
enum class HaulerState
{
    Idle,
    MovingToPickup,
    Loading,
    MovingToDelivery,
    Unloading
};
```

The truck **doesn't know why** the cargo is needed.

It only knows:

```text
pickup here
deliver there
```

That's exactly the separation you want.

---

# 19. Roads

Only after trucks work should roads matter.

Start with:

```text
off-road movement = expensive
road tile = cheap
```

The same A* algorithm can handle it using movement costs.

For example:

```text
grass cost = 5
road cost = 1
mud cost = 10
water = impossible
```

This creates emergent logistics naturally:

> Trucks technically can drive anywhere, but roads make them dramatically more efficient.

That's much easier to build than a rigid road-graph system initially.

---

# 20. Flow fields

Now you'll start seeing many vehicles heading toward the same depots.

That's when flow fields become interesting.

Instead of:

```text
truck1 → A*
truck2 → A*
truck3 → A*
...
truck100 → A*
```

you can compute one field:

```text
Every tile → Warehouse A
```

and vehicles follow it.

Red Blob Games has an excellent explanation of this exact many-agents-to-one-goal problem. [Red Blob Games](https://www.redblobgames.com/pathfinding/tower-defense/?utm_source=chatgpt.com)

[Red Blob Games — flow field pathfinding](https://www.redblobgames.com/pathfinding/tower-defense/?utm_source=chatgpt.com)

But this is an **optimization**, not phase-one navigation.

Don't implement it until profiling says A* traffic is actually becoming significant.

---

# 21. Rail building

Treat rails differently from roads.

Roads are grid traversal.

Rails should form a **graph**.

```cpp
struct RailNode
{
    Vector2 position;
    std::vector<RailEdgeId> edges;
};

struct RailEdge
{
    RailNodeId from;
    RailNodeId to;
    float length;
};
```

Then:

```text
Station A
   |
   |
 junction────Station B
   |
   |
Station C
```

is an actual graph rather than hundreds of independent tiles.

Use A*/Dijkstra on that graph.

---

# 22. Simplify trains aggressively

Don't initially simulate every locomotive coupling.

Make:

```cpp
struct Train
{
    RailPath path;
    float distanceAlongPath;

    std::vector<Wagon> wagons;
};
```

A visual wagon's position can simply be:

```text
train position - wagon offset along rail path
```

Gameplay sees one train entity.

Rendering sees six vehicles.

That saves you a huge physics problem.

---

# 23. Stations + train schedules

A train should have:

```cpp
struct TrainSchedule
{
    std::vector<TrainStopOrder> stops;
};
```

Example:

```text
Iron Mine
   ↓
Central Warehouse
   ↓
Factory District
   ↓
Iron Mine
```

Conditions can come later:

```text
cargo full
cargo empty
30 seconds passed
specific item count
```

Factorio's train scheduling design is worth studying for UI inspiration. [Factorio](https://factorio.com/blog/post/fff-279?utm_source=chatgpt.com)

---

# 24. Train blocks and signals

Do **not** solve train collisions by checking rectangle overlap.

Divide track into blocks.

```text
Signal
  ↓

======= BLOCK A ======= | ======= BLOCK B =======
```

A train must reserve/occupy a block before entering it.

The Factorio rail system uses block occupancy for exactly this reason; signals divide the rail network into blocks used to prevent multiple trains from colliding. [Official Factorio Wiki](https://wiki.factorio.com/Rail_signal?utm_source=chatgpt.com)

[Factorio railway signaling reference](https://wiki.factorio.com/Rail_signal?utm_source=chatgpt.com)

Also study OpenTTD.

Its path-signal system allows trains to reserve routes through intersections instead of locking an entire junction whenever only part of it is used. [OpenTTD Wiki](https://wiki.openttd.org/en/Manual/Signals?utm_source=chatgpt.com)

[OpenTTD signals guide](https://wiki.openttd.org/en/Manual/Signals?utm_source=chatgpt.com)

For version one of your game, though:

**use simple blocks first.**

Path reservations can come later.

---

# 25. Fog of war

Once basic warfare and logistics work, add:

```text
unexplored
explored
currently visible
```

Every entity capable of sight gets:

```cpp
struct Vision
{
    float range;
};
```

Maintain visibility per player on a coarse grid.

Don't raycast every unit against every tile.

Initially simply reveal a circle around each unit.

---

# 26. AI

Do AI much later than people instinctively want to.

AI should use the **same commands humans use**:

```text
MoveCommand
AttackCommand
BuildCommand
SetRecipeCommand
LogisticsPolicyCommand
```

The AI is basically another `PlayerController`.

```cpp
class HumanController;
class AIController;
```

Both produce:

```cpp
PlayerCommand
```

This is an extremely useful architectural constraint.

AI shouldn't have:

```cpp
enemyTank.teleportTo(...)
```

or direct access to internal systems.

---

# 27. Performance

Only optimize once you have a representative battle.

Test something like:

```text
500 combat units
100 trucks
20 trains
100 buildings
100 factories
20 warehouses
```

Then profile.

Your likely hotspots will be:

```text
pathfinding
unit separation
target acquisition
logistics matching
visibility
rendering
```

not:

```text
crafting recipes
inventories
health
```

At this stage your spatial grid becomes extremely important. [Game Programming Patterns](https://gameprogrammingpatterns.com/spatial-partition.html?utm_source=chatgpt.com)

EnTT's data-oriented ECS model will also help you query only the relevant components instead of walking giant polymorphic object graphs. [GitHub](https://github.com/skypjack/entt/wiki/Entity-Component-System?utm_source=chatgpt.com)

---

# 28. Testing

For this type of game, automated tests are unusually valuable because most of the interesting systems don't require graphics.

Use Catch2. Its current documentation includes CMake integration and conventional C++ unit testing. [GitHub](https://github.com/catchorg/catch2?utm_source=chatgpt.com)

Test things like:

```text
A* produces valid paths
warehouse reservations cannot exceed stock
recipe consumes correct ingredients
recipe produces correct output
truck delivery conserves items
destroyed truck loses its cargo
train cannot reserve occupied block
factory doesn't craft without ingredients
splash damage affects correct targets
```

You can eventually run thousands of simulated ticks without opening a raylib window.

That's a major advantage of keeping:

```text
simulation
```

separate from:

```text
renderer
```

---

# Architecture I would use

At the highest level:

```text
GameApp
│
├── Input
├── UI
├── Renderer
├── Assets
│
└── GameSession
    │
    ├── Simulation
    │   ├── Entity Registry
    │   ├── Definition Database
    │   ├── World
    │   ├── Command Buffer
    │   ├── Event Buffer
    │   └── Systems
    │
    └── Player Controllers
        ├── HumanController
        └── AIController
```

The important bit is:

```text
             ┌──────────── Renderer
             │
Input → Controller → Commands → SIMULATION
             │                  │
             └────── UI         └─ Events
```

**Renderer never owns gameplay state.**

**UI never changes gameplay state directly.**

**Input never changes gameplay state directly.**

Everything goes through the simulation.

---

# Core C++ classes

I'd roughly use:

```cpp
class GameApp
{
    GameSession session;
    Renderer renderer;
    InputManager input;
    UiManager ui;
};

class GameSession
{
    Simulation simulation;
    std::vector<std::unique_ptr<PlayerController>> players;
};

class Simulation
{
public:
    void tick();

private:
    entt::registry registry;

    World world;
    DefinitionDatabase definitions;

    PlayerCommandBuffer commands;
    SimulationEventBuffer events;

    PathService paths;
    LogisticsPlanner logistics;
    TrainScheduler trains;
};
```

And **systems**, not giant managers:

```cpp
class CommandSystem;
class NavigationSystem;
class MovementSystem;
class SeparationSystem;

class TargetAcquisitionSystem;
class WeaponSystem;
class ProjectileSystem;
class DamageSystem;

class ConstructionSystem;
class ProductionSystem;
class CraftingSystem;

class LogisticsSystem;
class TruckSystem;

class TrainMovementSystem;
class RailReservationSystem;

class VisionSystem;
class CleanupSystem;
```

---

# Components

Your initial component folder may eventually resemble:

```text
components/
    Transform.hpp
    Team.hpp
    Health.hpp
    Selectable.hpp

    GroundMover.hpp
    NavAgent.hpp
    OrderQueue.hpp

    Weapon.hpp
    Target.hpp
    Projectile.hpp

    Building.hpp
    ConstructionSite.hpp

    Inventory.hpp
    Crafter.hpp
    ProductionQueue.hpp

    LogisticsEndpoint.hpp
    Hauler.hpp

    Train.hpp
    TrainSchedule.hpp

    Vision.hpp
```

Components should mostly be **boring structs**.

Good:

```cpp
struct Health
{
    int current;
    int maximum;
};
```

Avoid:

```cpp
class HealthComponent
{
public:
    void CalculateDamageAgainstArmorAndNotifyRendererAnd...
};
```

The behavior belongs in systems.

---

# Static definitions versus runtime components

This distinction matters tremendously.

A tank definition:

```cpp
struct UnitDefinition
{
    UnitDefId id;
    int maxHealth;
    float movementSpeed;
    WeaponDefId weapon;
};
```

exists once.

Runtime tank:

```text
Entity 391
    Transform
    Health
    GroundMover
    Weapon
```

references that definition.

Don't copy:

```text
tank texture
description
base movement speed
weapon metadata
production recipe
```

onto every tank.

---

# Your world layer

I would give `World` several specialized structures:

```cpp
class World
{
public:
    TerrainGrid terrain;
    OccupancyGrid occupancy;
    NavigationGrid navigation;

    SpatialGrid entities;

    RailGraph rails;
};
```

Don't force one giant grid to solve all problems.

They answer different questions.

```text
TerrainGrid
    What is here?

OccupancyGrid
    Can I build here?

NavigationGrid
    Can this vehicle travel here?

SpatialGrid
    What entities are nearby?

RailGraph
    How are railway segments connected?
```

---

# Player commands

Your external simulation API should eventually look almost like this:

```cpp
using PlayerCommand = std::variant<
    MoveCommand,
    AttackCommand,
    AttackMoveCommand,

    PlaceBuildingCommand,
    CancelConstructionCommand,

    SetRecipeCommand,

    SetWarehousePolicyCommand,

    CreateTrainScheduleCommand
>;
```

This becomes incredibly powerful.

The same command stream can later drive:

```text
human players
AI
replays
network multiplayer
automated tests
```

---

# Events

Commands mean:

> Please do this.

Events mean:

> This happened.

Examples:

```cpp
UnitDestroyed
BuildingCompleted
RecipeFinished
CargoLoaded
CargoDelivered
TrainArrived
TrainBlocked
```

A small event system keeps systems from knowing too much about each other.

That decoupling is exactly the role described by the Event Queue pattern, though you should avoid turning one giant global event bus into the answer to everything. [Game Programming Patterns](https://gameprogrammingpatterns.com/event-queue.html?utm_source=chatgpt.com)

---

# Logistics architecture

This part deserves to be explicit.

Do this:

```text
                  LOGISTICS
                     │
          ┌──────────┴──────────┐
          ↓                     ↓
       SUPPLY                 DEMAND
          │                     │
 Warehouse A              Factory B
 300 iron free           wants 100 iron
          │                     │
          └──────────┬──────────┘
                     ↓
              LogisticsPlanner
                     ↓
                TransportJob
                     ↓
           Reserve 100 iron
                     ↓
                assign truck
                     ↓
             truck picks up
                     ↓
              truck delivers
                     ↓
             release reservation
```

Later the transporter might be:

```text
truck
train
ship
```

without changing how factories express demand.

That separation will save you a huge rewrite.

---

# Production architecture

Likewise:

```text
RecipeDefinition

IronPlateRecipe
    iron ore × 1
    ↓ 2 sec
    iron plate × 1
```

Then:

```text
FactoryEntity
    Inventory
    Crafter
```

`CraftingSystem` knows how to execute recipes.

It doesn't care whether the entity is visually:

```text
smelter
electronics factory
munitions factory
```

---

# Suggested folder structure

I'd start with:

```text
src/
│
├── main.cpp
│
├── app/
│   ├── GameApp.cpp
│   └── GameSession.cpp
│
├── sim/
│   ├── Simulation.cpp
│   │
│   ├── components/
│   │
│   ├── systems/
│   │
│   ├── commands/
│   └── events/
│
├── world/
│   ├── TerrainGrid.cpp
│   ├── NavigationGrid.cpp
│   ├── SpatialGrid.cpp
│   └── RailGraph.cpp
│
├── logistics/
│   ├── LogisticsPlanner.cpp
│   └── TransportJob.cpp
│
├── navigation/
│   ├── AStar.cpp
│   └── PathService.cpp
│
├── data/
│   ├── DefinitionDatabase.cpp
│   ├── UnitDefinition.hpp
│   ├── BuildingDefinition.hpp
│   ├── ItemDefinition.hpp
│   └── RecipeDefinition.hpp
│
├── rendering/
│   └── Renderer.cpp
│
├── ui/
│   ├── SelectionController.cpp
│   ├── BuildMenu.cpp
│   └── LogisticsPanel.cpp
│
└── tests/
```

---

# Don't build these early

There are several things I would deliberately ban from the first prototype:

```text
❌ multiplayer
❌ procedural maps
❌ modding API
❌ scripting language
❌ realistic train physics
❌ advanced shaders
❌ weather
❌ day/night
❌ terrain deformation
❌ electricity networks
❌ fluid simulation
❌ conveyor belts
❌ advanced formations
❌ hierarchical pathfinding
❌ sophisticated enemy AI
```

All of them are attractive distractions.

---

# Your first six playable milestones

If I were actually developing this, I'd organize development around **playable builds**, not technical subsystems:

| Build | What you should be able to do |
|---|---|
| **Prototype 1 — Toy RTS** | Select 20 rectangles and order them around |
| **Prototype 2 — Tiny C&C** | Build a base, produce tanks and fight an AI |
| **Prototype 3 — Industry** | Mine ore, refine it and craft electronics/ammo |
| **Prototype 4 — Trucks** | Warehouses automatically supply factories with trucks |
| **Prototype 5 — Rail** | Build track and automatically ship bulk cargo by train |
| **Prototype 6 — Actual Game** | Logistics economy powers an RTS battle |

**Prototype 2 should be genuinely fun before you start Prototype 3.**

That's important. Otherwise you can spend a year creating an impressive factory simulator only to discover that the actual RTS interaction isn't enjoyable.

And **Prototype 4 is the architectural proof point**. If a factory can say:

> “I need 20 electronics”

and your logistics layer can independently determine:

> “Warehouse 3 has them; assign Truck 17”

then you've built the core idea behind the game.

The eventual loop becomes:

```text
Capture resources
      ↓
Build extraction
      ↓
Move raw materials
      ↓
Process materials
      ↓
Move components
      ↓
Manufacture weapons/vehicles
      ↓
Expand logistics network
      ↓
Fight for territory/resources
      ↓
Destroy enemy logistics
      ↓
Repeat
```

That has a strong potential interaction between the **RTS layer and logistics layer**: instead of merely destroying someone's base, you can raid trucks, sever roads, knock out a rail bridge, capture a warehouse, or destroy their electronics production and indirectly cripple their army. That's where the C&C + Factorio idea becomes something meaningfully different rather than simply two games placed beside each other.

For the immediate next step, I'd implement only **Stages 1–4: CMake/raylib setup → fixed timestep → camera/world coordinates → box-selection/right-click commands**. Once those four are solid, introduce EnTT and start the real architecture.