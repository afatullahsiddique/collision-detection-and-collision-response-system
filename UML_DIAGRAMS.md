# UML Diagrams - Collision Detection System

## 1. Class Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                           Shape                                 │
│                      (Abstract Base)                            │
├─────────────────────────────────────────────────────────────────┤
│ Properties:                                                     │
│  - shapeType: ShapeType                                         │
├─────────────────────────────────────────────────────────────────┤
│ Methods:                                                        │
│  + getType(): ShapeType                                         │
│  + draw(renderer: Renderer*): void                             │
│  # constructor()                                                │
└─────────────────────────────────────────────────────────────────┘
         △                          △
         │                          │
         │ inherits                 │ inherits
         │                          │
    ┌────┴──────┐          ┌────────┴──────┐
    │            │          │               │
┌───┴────┐   ┌──┴─────┐  ┌─┴──────────┐  ┌─┴────────┐
│ Circle │   │Polygon │  │  Rectangle │  │  (other) │
├────────┤   ├────────┤  ├────────────┤  └──────────┘
│+ radius│   │+ vertex│  │+ width     │
└────────┘   │  list  │  │+ height    │
             └────────┘  └────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                       RigidBody                                 │
├─────────────────────────────────────────────────────────────────┤
│ Properties:                                                     │
│  - position: Vec2                                               │
│  - velocity: Vec2                                               │
│  - force: Vec2                                                  │
│  - mass: float                                                  │
│  - restitution: float                                           │
│  - dragCoefficient: float                                       │
│  - shape: unique_ptr<Shape>                                     │
├─────────────────────────────────────────────────────────────────┤
│ Methods:                                                        │
│  + integrate(dt: float): void                                   │
│  + applyForce(f: Vec2): void                                    │
│  + getShape(): Shape*                                           │
│  + setPosition(p: Vec2): void                                   │
│  + setVelocity(v: Vec2): void                                   │
└─────────────────────────────────────────────────────────────────┘
         △
         │ contains (composition)
         │
    ┌────┴────────────────────────────────────────┐
    │                                             │
    │ uses polymorphically                        │
    ▼                                             │
  Shape*                                      RigidBody[]
                                                  │
                                                  │ has
┌─────────────────────────────────────────────────┴───────────┐
│                    PhysicsWorld                              │
├──────────────────────────────────────────────────────────────┤
│ Properties:                                                  │
│  - bodies: vector<unique_ptr<RigidBody>>                     │
│  - gravity: Vec2                                             │
├──────────────────────────────────────────────────────────────┤
│ Methods:                                                     │
│  + step(dt: float): void                                     │
│  + addBody(body: unique_ptr<RigidBody>): void               │
│  + getBodies(): vector<RigidBody*>                          │
└──────────────────────────────────────────────────────────────┘
         △
         │ owns and manages
         │
┌────────┴─────────────────────────────────────────────────┐
│                      Engine                              │
├─────────────────────────────────────────────────────────┤
│ Properties:                                             │
│  - physicsWorld: PhysicsWorld                           │
│  - renderer: Renderer                                   │
│  - frameCount: int                                      │
│  - simulationTime: float                                │
├─────────────────────────────────────────────────────────┤
│ Methods:                                                │
│  + run(): void                                          │
│  - update(dt: float): void                              │
│  - render(): void                                       │
│  - processInput(): void                                 │
│  - checkBounds(): void                                  │
│  - detectCollisions(): void                             │
│  - resolveCollisions(): void                            │
│  - shutdown(): void                                     │
└─────────────────────────────────────────────────────────┘
         │
         ├─ uses (delegation) ──┐
         │                      │
         ▼                      ▼
    ┌─────────────┐    ┌──────────────────┐
    │  Renderer   │    │   Collision      │
    │  (OpenGL)   │    │  Detection &     │
    │ /PPM Output │    │  Response        │
    └─────────────┘    └──────────────────┘
         │                      │
         │ uses                 │ checks
         ▼                      ▼
      Shape::draw()        Manifold data
                           structure
```

---

## 2. Detailed Class Specifications

### Shape (Abstract Base Class)
```
┌─────────────────────────────────────────┐
│          <<abstract>>                   │
│             Shape                       │
├─────────────────────────────────────────┤
│ - shapeType: ShapeType {protected}      │
├─────────────────────────────────────────┤
│ + getType(): ShapeType {abstract}       │
│ + draw(Renderer*): void {abstract}      │
└─────────────────────────────────────────┘
```

### RigidBody (Physics Entity)
```
┌──────────────────────────────────────────────┐
│           RigidBody                          │
├──────────────────────────────────────────────┤
│ # position: Vec2                             │
│ # velocity: Vec2                             │
│ # force: Vec2                                │
│ # mass: float = 1.0                          │
│ # restitution: float = 0.8                   │
│ # dragCoefficient: float = 0.02              │
│ # shape: unique_ptr<Shape>                   │
├──────────────────────────────────────────────┤
│ + integrate(dt: float): void                 │
│ + applyForce(force: Vec2): void              │
│ + getShape(): Shape*                         │
│ + setPosition(p: Vec2): void                 │
│ + setVelocity(v: Vec2): void                 │
│ + getMass(): float                           │
│ + getPosition(): Vec2                        │
│ + getVelocity(): Vec2                        │
└──────────────────────────────────────────────┘
```

### Collision Detection Structures
```
┌────────────────────────────────────────┐
│         struct Manifold                │
├────────────────────────────────────────┤
│ + A: RigidBody*                        │
│ + B: RigidBody*                        │
│ + normal: Vec2                         │
│ + penetration: float                   │
├────────────────────────────────────────┤
│ Methods:                               │
│  (none - pure data structure)          │
└────────────────────────────────────────┘

Namespace: Collision Detection
├─ CheckCollision(Manifold&): bool
│  ├─ CircleCircle(Manifold&): bool
│  ├─ CircleRect(Manifold&): bool
│  ├─ RectCircle(Manifold&): bool
│  └─ RectRect(Manifold&): bool
│
└─ ResolveCollision(Manifold&): void
   (Impulse-based response calculation)
```

---

## 3. Component Diagram

```
┌─────────────────────────────────────────────────────────────┐
│         Collision Detection & Response System               │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌──────────────────────────────────────────────────────┐  │
│  │          Physics Simulation Component                │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │        PhysicsWorld                            │  │  │
│  │  │  • Manages gravity (0, 98.0 px/s²)            │  │  │
│  │  │  • Integrates all RigidBodies                 │  │  │
│  │  │  • Applies forces each timestep               │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────┘  │
│                          │                                  │
│                          ▼                                  │
│  ┌──────────────────────────────────────────────────────┐  │
│  │        Collision Detection Component                 │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │    Collision Detection Functions               │  │  │
│  │  │  • CircleCircle()                              │  │  │
│  │  │  • CircleRect()                                │  │  │
│  │  │  • RectRect()                                  │  │  │
│  │  │  • CheckCollision()                            │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────┘  │
│                          │                                  │
│                          ▼                                  │
│  ┌──────────────────────────────────────────────────────┐  │
│  │        Collision Response Component                  │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │    Impulse-Based Resolution                    │  │  │
│  │  │  • Calculate relative velocity                 │  │  │
│  │  │  • Compute impulse magnitude                   │  │  │
│  │  │  • Update velocities                           │  │  │
│  │  │  • ResolveCollision()                          │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────┘  │
│                          │                                  │
│                          ▼                                  │
│  ┌──────────────────────────────────────────────────────┐  │
│  │        Rendering Component                           │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │    Renderer (Software Rasterization)           │  │  │
│  │  │  • 1200x800 framebuffer                        │  │  │
│  │  │  • Circle drawing algorithm                    │  │  │
│  │  │  • PPM file output                             │  │  │
│  │  │  • ImageWriter (PPM encoding)                  │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────┘  │
│                          │                                  │
│                          ▼                                  │
│              Output: collision_frame_*.ppm                  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## 4. Sequence Diagram: Collision Detection Pipeline

```
Main Loop                PhysicsWorld              Collision         Response
   │                         │                         │                │
   ├─ step(dt) ──────────────>│                         │                │
   │                         │                         │                │
   │                ┌────────────────────────────┐      │                │
   │                │ Integrate all RigidBodies  │      │                │
   │                │ • Apply forces (gravity)   │      │                │
   │                │ • Calculate acceleration   │      │                │
   │                │ • Update velocity & pos    │      │                │
   │                └────────────────────────────┘      │                │
   │                         │                         │                │
   │                         ◄─ return                  │                │
   │                         │                         │                │
   ├─ detectCollisions ──────>│─ Check all pairs ──────>│                │
   │                         │                         │                │
   │                         │             ┌─────────────────────┐      │
   │                         │             │ CircleCircle Check  │      │
   │                         │             │ • Calculate distance│      │
   │                         │             │ • Test vs sum(radii)│      │
   │                         │             └─────────────────────┘      │
   │                         │                         │                │
   │                         │             ┌─────────────────────┐      │
   │                         │             │ RectRect Check      │      │
   │                         │             │ • Test AABB overlap │      │
   │                         │             │ • Calc penetration  │      │
   │                         │             └─────────────────────┘      │
   │                         │                         │                │
   │                         │<─ Manifold objects ────│                │
   │                         │                         │                │
   ├─ resolveCollisions ───────────────────────────────────────────────>│
   │                         │                         │    ┌────────────────────────┐
   │                         │                         │    │ For each Manifold:     │
   │                         │                         │    │ • Calc relative vel    │
   │                         │                         │    │ • Compute impulse      │
   │                         │                         │    │ • Apply to bodies      │
   │                         │                         │    │ • Update velocities    │
   │                         │                         │    └────────────────────────┘
   │                         │                         │                │
   │                         │                         │<──────────────│
   │                         │                         │                │
   ├─ checkBounds ────────────────────────────────────────────────────>│ Physics Update
   │                         │                         │                │
   │                  (Boundary collision)             │                │
   │                  (Velocity reversal)              │                │
   │                         │                         │                │
   ├─ render() ──────────────>│ Get body data          │                │
   │                         │ (pos, shape, color)    │                │
   │                         │                         │                │
   │                    [Display framebuffer or PPM file]              │
   │                         │                         │                │
```

---

## 5. State Diagram: RigidBody Physics State

```
         ┌─────────────────────────────────────┐
         │       RigidBody Physics State        │
         └─────────────────────────────────────┘
                          ▲
                          │
                    ┌─────┴─────┐
                    │           │
          (dt)      │           │      (dt)
          ┌─────────┘           └──────────┐
          │                                │
    ┌─────▼─────┐                  ┌──────▼───┐
    │ Integrate │──(update)───────>│ Collide  │
    │ Physics   │                  │ Check    │
    │ Forces    │                  └──────┬───┘
    └─────┬─────┘                         │
          │                        (collision)
          │                         ┌──────┴───┐
          │                         │           │
          │                   ┌─────▼────┐  ┌──▼──────────┐
          │                   │  Resolve │  │ No Collision│
          │                   │ Collision│  │  (Continue) │
          │                   └─────┬────┘  └──┬──────────┘
          │                         │           │
          └──────────────┬──────────┴───────────┘
                         │
                    (repeat)
                         │
                    ┌────▼──────┐
                    │  Boundary │
                    │ Collision?│
                    └────┬──────┘
                         │
            ┌────────────┼────────────┐
            │            │            │
       ┌────▼───┐   ┌────▼───┐  ┌────▼───┐
       │ Bounce │   │Continue│  │Continue│
       │ Reverse│   │(no wall)   │ (wall) │
       │Velocity│   └────┬───┘  └────┬───┘
       └────┬───┘        │           │
            └────────────┴───────────┘
                     │
                (next frame)
```

---

## 6. Activity Diagram: Simulation Update Loop

```
        (Start) ┌─────┐
           │    │     │
           └───>│ Run │
                │     │
                └──┬──┘
                   │
           ┌───────▼────────┐
           │ While Running? │
           │ (time < 10s)   │
           └───┬────────┬───┘
               │        │
            Yes│        │No
               │        └──────────┐
               │                   │
        ┌──────▼──────┐            │
        │Process Input│            │
        └──────┬──────┘            │
               │                   │
        ┌──────▼──────────┐        │
        │Apply Forces     │        │
        │• Gravity        │        │
        │• Drag           │        │
        │• Applied Forces │        │
        └──────┬──────────┘        │
               │                   │
        ┌──────▼──────────┐        │
        │Integrate Physics│        │
        │• v += a*dt      │        │
        │• p += v*dt      │        │
        └──────┬──────────┘        │
               │                   │
        ┌──────▼──────────────┐    │
        │Detect Collisions    │    │
        │• Check all pairs    │    │
        │• Build manifolds    │    │
        └──────┬──────────────┘    │
               │                   │
        ┌──────▼──────────────┐    │
        │Resolve Collisions   │    │
        │• Impulse-based      │    │
        │• Update velocities  │    │
        └──────┬──────────────┘    │
               │                   │
        ┌──────▼──────────────┐    │
        │Check Boundaries     │    │
        │• Wall collisions    │    │
        │• Clamp positions    │    │
        └──────┬──────────────┘    │
               │                   │
        ┌──────▼──────────────┐    │
        │Render Scene         │    │
        │• Draw bodies        │    │
        │• Save PPM frame     │    │
        │• Update display     │    │
        └──────┬──────────────┘    │
               │                   │
        ┌──────▼──────────────┐    │
        │Increment Time       │    │
        │ t += dt (0.016s)    │    │
        └──────┬──────────────┘    │
               │                   │
        ┌──────▼──────────┐        │
        │Loop Back?       │        │
        └───┬────────┬────┘        │
            │        │             │
         Yes│        │No           │
            │        │             │
            └────┬───┴─────────────┘
                 │
          ┌──────▼──────┐
          │Shutdown     │
          │• Close files│
          │• Free memory│
          └──────┬──────┘
                 │
            (End)└─────┐
                       ▼
                    STOP
```

---

## 7. Collaboration Diagram

```
              ┌─────────────────┐
              │     Engine      │
              │  (Controller)   │
              └────────┬────────┘
                       │
         ┌─────────────┼──────────────────┐
         │             │                  │
         ▼             ▼                  ▼
    ┌────────┐  ┌─────────────┐  ┌────────────┐
    │Physics │  │ Collision   │  │ Renderer   │
    │ World  │  │ Detection   │  │ (Software) │
    │        │  │             │  │            │
    │ "1:step│  │ "2:detect   │  │ "7:render" │
    │  bodies│  │  collisions"│  │            │
    │"        │  │             │  │            │
    └───┬────┘  └──┬──────┬────┘  └──┬─────────┘
        │          │      │          │
        │          │      │     ┌────▼───────┐
        │          │      │     │ ImageWriter│
        │          │      │     │  (PPM out) │
        │          │      │     └────────────┘
        │          │      └────────┬─────────┐
        │          │               │         │
        ▼          ▼               ▼         ▼
    ┌──────────┐ ┌──────────────┐ ┌─────┐  ┌──────┐
    │RigidBody │ │   Manifold   │ │Shape│  │File  │
    │Objects   │ │  (collision) │ │Data │  │ PPM  │
    │         │ │   contacts   │ │     │  │      │
    └──────────┘ └──────────────┘ └─────┘  └──────┘
         │
         └────────┬──────────┬─────────────────┐
                  │          │                 │
                  ▼          ▼                 ▼
             ┌────────┐ ┌────────┐     ┌────────────┐
             │ Circle │ │Polygon │     │ Rectangle  │
             │ Shape  │ │ Shape  │     │  Shape     │
             └────────┘ └────────┘     └────────────┘
```

---

## 8. Object Interaction Example: Circle Collision

### Scenario: Two circles collide

```
Client (Engine)
    │
    ├─ step() ──────────────────────────> PhysicsWorld
    │                                         │
    │                                         ├─ Integrate CircleA
    │                                         │  pos = (300, 150)
    │                                         │  vel = (200, 100)
    │                                         │
    │                                         ├─ Integrate CircleB
    │                                         │  pos = (900, 150)
    │                                         │  vel = (-200, 100)
    │                                         │
    │                                         └─ return
    │
    ├─ detectCollisions() ───────────────> CheckCollision()
    │                                         │
    │                                         ├─ CircleA vs CircleB
    │                                         │  distance = sqrt((900-300)² + 0²) = 600
    │                                         │  sum_radii = 30 + 30 = 60
    │                                         │
    │                                         ├─ Check: distance (600) >= sum_radii (60)?
    │                                         │  Yes → No collision detected
    │                                         │
    │                                         └─ (later frames...)
    │                                         
    │                                         ├─ Relative position (frame 150)
    │                                         │  distance = 50 pixels
    │                                         │  
    │                                         ├─ Check: 50 >= 60?
    │                                         │  No → COLLISION!
    │                                         │
    │                                         ├─ Build Manifold:
    │                                         │  m.A = CircleA
    │                                         │  m.B = CircleB
    │                                         │  m.normal = (1, 0)  [A toward B]
    │                                         │  m.penetration = 60 - 50 = 10
    │                                         │
    │                                         └─ return true
    │
    ├─ resolveCollisions() ──────────────> ResolveCollision(manifold)
                                               │
                                               ├─ rv = CircleB.vel - CircleA.vel
                                               │    = (-200, 100) - (200, 100)
                                               │    = (-400, 0)
                                               │
                                               ├─ vel_along_normal = (-400, 0) · (1, 0)
                                               │                   = -400
                                               │
                                               ├─ Restitution e = min(0.8, 0.8) = 0.8
                                               │
                                               ├─ j = -(1 + 0.8) * (-400) / (1/1 + 1/1)
                                               │    = -1.8 * (-400) / 2
                                               │    = 720 / 2 = 360
                                               │
                                               ├─ impulse = (1, 0) * 360 = (360, 0)
                                               │
                                               ├─ CircleA.vel = (200, 100) - (360, 0)/1
                                               │              = (-160, 100)
                                               │
                                               ├─ CircleB.vel = (-200, 100) + (360, 0)/1
                                               │              = (160, 100)
                                               │
                                               └─ return
```

---

## 9. Entity-Relationship Diagram

```
┌────────────────┐         1:N      ┌────────────────┐
│ PhysicsWorld   │─────────────────>│   RigidBody    │
│                │   contains        │                │
│ • gravity      │                   │ • position     │
│ • time step    │                   │ • velocity     │
│                │                   │ • mass         │
└────────────────┘                   │ • restitution  │
                                     │ • drag         │
                                     └────────┬───────┘
                                              │
                                         1:1  │ has
                                              │
                                         ┌────▼────────┐
                                         │   Shape     │
                                         │             │
                                         │ • shapeType │
                                         │             │
                                         └─────────────┘
                                         
┌────────────────┐        N:M      ┌───────────────────┐
│   RigidBody    │────collision──>│  RigidBody        │
│                │   contact       │ (other body)      │
└────────────────┘                 └───────────────────┘
      │                                     │
      │ creates                             │
      │                                     │
      └──────────┬──────────────────────────┘
                 │
            ┌────▼────────┐
            │  Manifold   │
            │             │
            │ • normal    │
            │ • penetr.   │
            └─────────────┘
```

---

## 10. Deployment Diagram

```
┌─────────────────────────────────────────────────────────┐
│                   Development System                    │
│                                                         │
│  ┌──────────────────────────────────────────────────┐  │
│  │         Source Code (.cpp, .h files)            │  │
│  │  • Engine.cpp/h                                 │  │
│  │  • Collision.cpp/h                              │  │
│  │  • PhysicsWorld.cpp/h                           │  │
│  │  • RigidBody.cpp/h                              │  │
│  │  • Renderer.cpp/h                               │  │
│  └──────────────────────────────────────────────────┘  │
│                         │                              │
│                    CMake Build                         │
│                         │                              │
│  ┌──────────────────────▼──────────────────────────┐  │
│  │           Compilation & Linking                 │  │
│  │  • GCC/Clang C++17 Compiler                    │  │
│  │  • OpenGL library linking                       │  │
│  │  • GLFW library linking                         │  │
│  └──────────────────────┬──────────────────────────┘  │
│                         │                              │
│                         ▼                              │
│  ┌──────────────────────────────────────────────────┐  │
│  │        Executable: CollisionEngine              │  │
│  │  • Binary object files                          │  │
│  │  • Linked runtime libraries                     │  │
│  │  • ~500 KB executable size                      │  │
│  └──────────────────────────────────────────────────┘  │
│                         │                              │
└─────────────────────────┼──────────────────────────────┘
                          │ runs
                          │
┌─────────────────────────▼──────────────────────────────┐
│              Runtime Environment                       │
│                                                        │
│  ┌──────────────────────────────────────────────────┐ │
│  │         System Resources                        │ │
│  │  • CPU: Multi-core x86_64                      │ │
│  │  • Memory: 512 MB (typical usage)              │ │
│  │  • OS: Linux, macOS, Windows (with CMake)      │ │
│  │  • Libraries: OpenGL, GLFW3, libstdc++         │ │
│  └──────────────────────────────────────────────────┘ │
│                         │                              │
│  ┌──────────────────────▼──────────────────────────┐ │
│  │         Output Artifacts                        │ │
│  │  • /tmp/collision_frame_*.ppm (20 files)      │ │
│  │  • Console output logs                         │ │
│  │  • Framebuffer data (1200×800 RGB)            │ │
│  └──────────────────────────────────────────────────┘ │
│                         │                              │
└─────────────────────────┼──────────────────────────────┘
                          │ generates
                          │
                ┌─────────▼─────────┐
                │  PPM Image Files  │
                │ (for visualization)│
                │  • RGB format     │
                │  • Portable       │
                │  • Human readable │
                └───────────────────┘
```

---

## 11. Usage Notes for StarUML

To create these diagrams in StarUML:

### Class Diagram Creation:
1. Create classes for: Shape, Circle, Rectangle, RigidBody, PhysicsWorld, Engine, Collision
2. Add properties and methods for each class
3. Create inheritance relationships (|--) for Circle/Rectangle → Shape
4. Add composition relationships (--*) for RigidBody → Shape, Engine → PhysicsWorld
5. Add association arrows (-->) for Engine → Renderer, Engine → Collision detection

### Sequence Diagram:
1. Create actors/objects: Engine, PhysicsWorld, Collision, RigidBody
2. Add messages for each simulation step (step, detect, resolve)
3. Use self-calls (loops) for body integration
4. Show parallel execution of collision checks

### Component Diagram:
1. Create components for Physics, Collision, Rendering
2. Show dependencies and data flow
3. Include external libraries (OpenGL, GLFW)

### Activity Diagram:
1. Start/End nodes
2. Actions for each phase of simulation
3. Decision nodes (conditional branching)
4. Join/split bars for parallel processing

---

## 12. Pattern Application

### Design Patterns Used

| Pattern | Location | Benefit |
|---------|----------|---------|
| **Strategy** | Shape/Collision detection dispatch | Different algorithms per shape pair |
| **Template Method** | RigidBody integration pattern | Common physics pipeline, customizable |
| **Factory** | Shape creation | Decouples shape instantiation |
| **Observer** | (implicit) | Collision manifolds notify resolution |
| **Composite** | Shape hierarchy | Polymorphic rendering/collision |

---

**UML Diagram Set Generated**: 12 diagrams covering all aspects of system architecture  
**Suitable for**: Formal project submission, documentation, design review

