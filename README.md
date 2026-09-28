# N-Body Simulator

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/CUDA-NVIDIA-76B900?style=for-the-badge&logo=nvidia&logoColor=white" />
  <img src="https://img.shields.io/badge/CMake-3.20%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white" />
</p>

This started as a gravity simulator I wrote in high school. I found it again recently and started rewriting it in C++.

Right now it uses the direct **O(N²)** method: every body checks every other body. I am keeping that version around as the reference while I work on the CUDA version, numerical accuracy, and eventually Barnes-Hut.

There are faster N-body implementations already.

That is not really the point.

I am using the simple solver as a controlled environment to work through those problems instead of jumping directly to the final optimization.

## Current State

The simulator currently has:

- a direct **O(N²)** CPU implementation
- a direct **O(N²)** CUDA implementation
- velocity Verlet integration
- deterministic body generation
- CPU, GPU, and combined run modes
- position, velocity, and acceleration output

The CUDA version currently gives one body to each thread. Each thread loops over the other bodies and adds up the gravitational acceleration acting on its body.

It works, but it is still the first pass. Device allocation, memory copies, synchronization, and cleanup all happen inside each timestep.

## Numerical Method

For body $i$, acceleration is calculated from every other body:

```math
\mathbf{a}_i =
G \sum_{j \ne i}
m_j
\frac{\mathbf{r}_j - \mathbf{r}_i}
{\left|\mathbf{r}_j - \mathbf{r}_i\right|^3}
```

The simulator uses velocity Verlet integration:

```math
\mathbf{x}_{t+\Delta t}
=
\mathbf{x}_t
+
\mathbf{v}_t \Delta t
+
\frac{1}{2}\mathbf{a}_t \Delta t^2
```

Then, after recalculating acceleration at the new position:

```math
\mathbf{v}_{t+\Delta t}
=
\mathbf{v}_t
+
\frac{1}{2}
\left(
\mathbf{a}_t + \mathbf{a}_{t+\Delta t}
\right)
\Delta t
```

I want to use this to look at how timestep size, close encounters, and implementation differences affect the simulation instead of only caring about runtime.

## CUDA

The current CUDA step is split into three kernels:

1. update positions
2. calculate accelerations
3. update velocities

The acceleration kernel is still the direct method:

```cpp
for (int j = 0; j < n; ++j) {
    if (i == j) continue;

    displacement.x = bodies[j].position.x - bodies[i].position.x;
    displacement.y = bodies[j].position.y - bodies[i].position.y;
    displacement.z = bodies[j].position.z - bodies[i].position.z;

    distance = sqrt(
        displacement.x * displacement.x +
        displacement.y * displacement.y +
        displacement.z * displacement.z
    );

    factor = G * bodies[j].mass /
             (distance * distance * distance);

    ax += factor * displacement.x;
    ay += factor * displacement.y;
    az += factor * displacement.z;
}
```

No shared-memory tiling or spatial approximation yet.

## Build

Requirements:

- CMake 3.20+
- C++20 compiler
- NVIDIA CUDA Toolkit
- CUDA-capable NVIDIA GPU

```bash
cmake -S . -B build
cmake --build build --config Release
```

## Run

```text
nbodysim <steps> <timestep> <bodies> <mode>
```

Modes:

```text
cpu
gpu
both
```

Short flags also work:

```text
--c
--g
--b
```

Example:

```bash
nbodysim 1000 60 1000 gpu
```

Body state is written to `output.txt`.

## Measurements

The first timing numbers I collected were not useful enough to keep.

The CUDA timer currently includes allocation, host/device copies, synchronization, and deallocation along with the kernel work. That tells me how long the whole timestep takes, but not where the time is actually going.

Before putting new numbers here, I want to:

- keep device allocations alive across timesteps
- time kernels with CUDA events
- separate kernel time from transfer and allocation overhead
- compare CPU and CUDA runs under the same initial conditions

## Roadmap

### Numerical checks

Add:

- kinetic energy
- gravitational potential energy
- total system energy
- energy drift
- CPU/CUDA error comparison
- close-encounter tests

### CUDA cleanup and profiling

Move allocations out of the timestep loop, reduce unnecessary transfers, and profile the direct solver before changing the algorithm.

### Performance work

After the measurements are trustworthy, I want to look at memory access, shared-memory tiling, and the actual bottlenecks in the direct CUDA implementation.

### Barnes-Hut

Once I understand its numerical behavior and GPU bottlenecks, I want to implement Barnes-Hut and compare the tradeoff between approximation error and the reduction in computational work.
