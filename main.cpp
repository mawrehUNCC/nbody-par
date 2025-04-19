#include <iostream>
#include <fstream>
#include <omp.h>
#include "nbody.hpp"

int main(int argc, char* argv[]) {
    // Arguments: dt and number of steps
    double dt = 1.0;
    int steps = 10000;
    if (argc > 1) {
        dt = std::stod(argv[1]); // Get dt from command line arguments
    }
    if (argc > 2) {
        steps = std::stoi(argv[2]); // Get steps from command line arguments
    }

    // Initialize the simulation object
    NBodySimulation sim(dt, 0.0); // Using softening factor of 0.0 for simplicity

    // Measure time for different scenarios

    // Scenario 1: Simulate Solar System with dt = 200 for 5,000,000 steps
    if (dt == 200.0 && steps == 5000000) {
        sim.initSolarSystem();  // Initialize the solar system setup

        double startTime = omp_get_wtime();  // Start timing

        for (int i = 0; i < steps; ++i) {
            sim.resetForces();
            sim.computeForcesParallel();
            sim.integrateMotionParallel();
        }

        double endTime = omp_get_wtime();  // End timing
        double elapsedTime = endTime - startTime;
        std::cout << "Time for simulating the solar system: " << elapsedTime << " seconds." << std::endl;
        sim.outputState(std::cout);  // Output final state
    }
    // Scenario 2: Simulate 100 particles with dt = 1 for 10,000 steps
    else if (steps == 10000 && dt == 1.0) {
        sim.initRandom(100);  // Initialize 100 random particles

        double startTime = omp_get_wtime();  // Start timing

        for (int i = 0; i < steps; ++i) {
            sim.resetForces();
            sim.computeForcesParallel();
            sim.integrateMotionParallel();
        }

        double endTime = omp_get_wtime();  // End timing
        double elapsedTime = endTime - startTime;
        std::cout << "Time for simulating 100 particles: " << elapsedTime << " seconds." << std::endl;
        sim.outputState(std::cout);  // Output final state
    }
    // Scenario 3: Simulate 1000 particles with dt = 1 for 10,000 steps
    else if (steps == 10000 && dt == 1.0) {
        sim.initRandom(1000);  // Initialize 1000 random particles

        double startTime = omp_get_wtime();  // Start timing

        for (int i = 0; i < steps; ++i) {
            sim.resetForces();
            sim.computeForcesParallel();
            sim.integrateMotionParallel();
        }

        double endTime = omp_get_wtime();  // End timing
        double elapsedTime = endTime - startTime;
        std::cout << "Time for simulating 1000 particles: " << elapsedTime << " seconds." << std::endl;
        sim.outputState(std::cout);  // Output final state
    }

    return 0;
}
