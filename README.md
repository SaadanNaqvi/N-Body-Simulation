# N-Body Simulation

A real-time 3D N-body gravitational simulation written in C++ and OpenGL.

This project simulates gravitational interactions between hundreds to thousands of particles using a physically based force model and renders them in real time with interactive camera controls. The focus of the project is both numerical stability and algorithmic performance, progressing from a naïve O(nlogn) solver to an optimized O(nlogn) Barnes Hut implementation.

---

## Features

- Newtonian gravity between all particles
- Velocity Verlet integration for stable time stepping
- Real time 3D rendering using OpenGL
- Motion trails to visualize trajectories
- Free-fly camera with mouse + keyboard controls
- Dynamic system respawning at runtime
- Barnes Hut octree acceleration structure (this took me way too long to implement)
- Multi-threaded force evaluation (CPU parallelism)

---

## Benchmarking

### Bruteforce
<p align="center">
  <img src="media/n=200 FIRST.gif" alt="N-body simulation n=200" width="700"/>
</p>

<p align="center">
  <em>n = 350 particles · FPS ≈ 50 · TC: O(n^2) per frame</em>
</p>


### Barnes Hut Optimised Solver
<p align="center">
  <img src="media/n=2000 Second No Trail.gif" alt="N-body simulation n=2000" width="700"/>
</p>

<p align="center">
  <em>n = 2,000 particles · FPS ≈ 50 · TC: O(n*log(n)) per frame · Barnes Hut Algorithm</em>
</p>


### Barnes Hut Optimised Solve + Multi-Threading + Veolcity Verlet Optimisations
<p align="center">
  <img src="media/n=10000 Third No trail.gif" alt="N-body simulation n=10000" width="700"/>
</p>

<p align="center">
  <em>n = 10,000 particles · FPS ≈ 50 · TC: O(n*log(n)) per frame · Barnes Hut Algorithm + Multi-Threading + Velocity Verlet Optimisations</em>
</p>

---


**Configuration**
- Force model: Newtonian gravity
- Integration: Velocity Verlet
- Acceleration structure: Barnes Hut Octree
- Parallelism: Multi-threaded CPU force evaluation
- Hardware: MacBook Pro 2023 (Apple M3, 8 GB RAM)

## Controls

| Key | Action |
|---|---|
| W / A / S / D | Move camera |
| Space / Shift | Move up / down |
| Mouse | Look around |
| Scroll | Zoom |
| R | Randomly respawn system |
| Esc | Exit |

---

## How It Works (High Level)

- Each particle has mass, position, velocity, and radius
- Gravitational forces are computed pairwise using Newton’s law of gravitation
- Net forces are integrated using Velocity Verlet
- Positions are scaled for visualization while preserving physical realism
- For large systems, distant particle clusters are approximated using their center of mass via a Barnes Hut octree

---

## Physics Model

The simulation is based on classical Newtonian mechanics.

### Gravitational Force

Each pair of particles interacts via Newton’s law of universal gravitation:

$$\mathbf{F}_{ij} = G \frac{m_i m_j}{\|\mathbf{r}_{ij}\|^2} \mathbf{\hat{r}}_{ij}$$

Where:
- $G$ is the gravitational constant  
- $m_i, m_j$ are particle masses  
- $\mathbf{r}_{ij}$ is the distance between particles  
- $\mathbf{\hat{r}}_{ij}$ is the unit direction vector from particle i to j  

The net force acting on a particle is the sum of forces from all other particles:

$$\mathbf{F}_i = \sum_{j \neq i} \mathbf{F}_{ij}$$

### Equations of Motion

Acceleration is computed using Newton’s second law:

$$\mathbf{a} = \frac{\mathbf{F}}{m}$$

### Time Integration

The system is advanced in time using Velocity Verlet integration, which provides improved numerical stability over explicit Euler methods.

Position Update:

$$\mathbf{x}(t + \Delta t) = \mathbf{x}(t) + \mathbf{v}(t)\Delta t + \frac{1}{2}\mathbf{a}(t)\Delta t^2$$

Velcotiy Update:

$$\mathbf{v}(t + \Delta t) = \mathbf{v}(t) + \frac{1}{2}(\mathbf{a}(t) + \mathbf{a}(t + \Delta t))\Delta t$$

## Performance

- Brute-force solver scales as O(n²) and becomes impractical beyond a few hundred particles
- Barnes Hut reduces force evaluation to approximately O(n log n)
- Multi-threading further improves scalability on modern multi core CPUs
- Rendering becomes the primary bottleneck at very large particle counts

---

## How to Run

### First Time Setup
```bash
cmake -S . -B build
```

### Compile and then run
```bash
cmake --build build

./build/simulation 
```
