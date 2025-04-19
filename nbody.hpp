#ifndef NBODY_HPP
#define NBODY_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <omp.h>

// Vector3D structure to hold position, velocity, and force vectors
struct Vec3 {
    double x, y, z;

    Vec3 operator+(const Vec3& rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vec3 operator-(const Vec3& rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Vec3 operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vec3 operator/(double scalar) const { // Added division operator
        return {x / scalar, y / scalar, z / scalar};
    }

    Vec3& operator+=(const Vec3& rhs) {
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& rhs) { // Added subtraction assignment operator
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }

    double norm() const {
        return std::sqrt(x*x + y*y + z*z);
    }
};

// Particle structure to hold mass, position, velocity, and force of each particle
struct Particle {
    double mass;
    Vec3 position, velocity, force;
};

class NBodySimulation {
private:
    double dt; // Time step
    double softening; // Softening factor to prevent singularity in gravitational force
    std::vector<Particle> particles;

public:
    NBodySimulation(double timestep, double softening) : dt(timestep), softening(softening) {}

    void initRandom(int numParticles);
    void initSolarSystem();
    void resetForces();
    void computeForcesParallel();
    void integrateMotionParallel();
    void outputState(std::ostream& out);
    void loadFromFile(const std::string& filename);
};

#endif // NBODY_HPP
