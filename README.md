# N-Body Simulation

A real time 3D N-body gravitational simulation written in C++ and OpenGL.

This project simulates gravitational interactions between hundreds of particles using a physically based force model and renders them in real time with camera controls and motion trails.

The simulation supports dynamic spawning, interactive camera movement, and real-time performance tracking (FPS).

---

## Features

- Newtonian gravity between all particles
- Velocity Verlet integration for stable time stepping
- Real time 3D rendering using OpenGL
- Motion trails to visualize trajectories
- Free-fly camera with mouse + keyboard controls
- Random system respawn at runtime

---

## Benchmarking

<p align="center">
  <img src="media/n=200 FIRST.gif" alt="N-body simulation n=200" width="700"/>
</p>

<p align="center">
  <em>n = 200 particles · FPS ≈ 60 · V1 · no optimisations</em>
</p>

---


**Configuration**
- Force model: Newtonian gravity
- Integration: Velocity Verlet
- Acceleration structure: None
- Hardware: MacBook Pro 2023 Apple M3, 8G RAM

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

---

## Physics Model

The simulation is based on classical Newtonian mechanics.

### Gravitational Force

Each pair of particles interacts via Newton’s law of universal gravitation.

### Equations of Motion

Acceleration is computed using Newton’s second law.

### Time Integration

The system is advanced in time using Velocity Verlet integration, which provides improved numerical stability over explicit Euler methods.



## Performance

- Handles hundreds of particles in real time

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
