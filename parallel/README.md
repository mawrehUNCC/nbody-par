# Parallel N-Body Simulation

This is a parallel N-body simulation using OpenMP in C++. It simulates gravitational interactions between particles.

## How to Build

cd parallel
make

How to Run
./nbody_parallel <num_particles|'planet'|filename> <dt> <steps> <output_interval>

Simulate 100 particles for 1000 steps, time step = 1, output every 100 steps:./nbody_parallel 100 1 1000 100

Output Format
Each line represents one time step.
First column: number of particles
Then for each particle: mass, x, y, z, vx, vy, vz, fx, fy, fz
Values are tab-separated (.tsv format)