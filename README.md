
# N-Body Simulation

This project simulates the motion of particlesin space using the N-body problem. It supports parallel processing using OpenMP to simulate the interaction of particles based on their gravitational forces.

## Compilation

1. Clone the repository or download the source files.

2. Open a terminal and navigate to the project directory.

3. To compile the program, run the following command:

   ```bash
   g++ -std=c++11 -fopenmp -O2 -o nbody main.cpp nbody.cpp
   ```

   This command compiles the `main.cpp` and `nbody.cpp` source files, linking them into an executable named `nbody`.

## Running the Simulation

### For the Solar System Simulation:
To simulate the solar system with a time step (`dt`) of 200 for 5,000,000 steps, use the following command:

```bash
./nbody 200 5000000
```

### For Custom Simulations:
You can modify the program to simulate different numbers of particles and time steps by adjusting the arguments in the code or changing the parameters directly in the `main.cpp` file.

## Notes

- The program uses `rand()` for random nitialization of particle positions and velocities.
- The simulation supports parallel computation of forces between particles using OpenMP.
