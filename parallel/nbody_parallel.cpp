#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <omp.h>
#include <chrono>
#include <string>

const double G = 6.674e-11; // Gravitational constant
const double SOFTENING = 1e-9; // Softening factor to prevent instability

// Vector structure for position, velocity, force
struct Vec3 {
    double x, y, z;

    Vec3 operator+(const Vec3& b) const { return {x + b.x, y + b.y, z + b.z}; }
    Vec3 operator-(const Vec3& b) const { return {x - b.x, y - b.y, z - b.z}; }
    Vec3 operator*(double s) const { return {x * s, y * s, z * s}; }
    Vec3 operator/(double s) const { return {x / s, y / s, z / s}; }

    void operator+=(const Vec3& b) { x += b.x; y += b.y; z += b.z; }
    void reset() { x = y = z = 0; }

    double norm_squared() const { return x*x + y*y + z*z; }
};

// Particle structure to store mass, position, velocity, and force
struct Particle {
    double mass;
    Vec3 pos, vel, force;
};

// Function to compute forces on particles
void compute_forces(std::vector<Particle>& particles) {
    int n = particles.size();
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < n; ++i) {
        Vec3 total_force = {0, 0, 0};
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            Vec3 dir = particles[j].pos - particles[i].pos;
            double dist_sqr = dir.norm_squared() + SOFTENING;
            double force_mag = (G * particles[i].mass * particles[j].mass) / dist_sqr;
            double dist = std::sqrt(dist_sqr);
            Vec3 unit_dir = dir / dist;
            total_force += unit_dir * force_mag;
        }
        particles[i].force = total_force;
    }
}

// Function to update positions and velocities
void update_particles(std::vector<Particle>& particles, double dt) {
    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); ++i) {
        Vec3 acc = particles[i].force / particles[i].mass;
        particles[i].vel += acc * dt;
        particles[i].pos += particles[i].vel * dt;
    }
}

// Function to output the state of the system (positions, velocities, forces)
void output_state(const std::vector<Particle>& particles) {
    std::cout << particles.size();
    for (const auto& p : particles) {
        std::cout << "\t" << p.mass
                  << "\t" << p.pos.x << "\t" << p.pos.y << "\t" << p.pos.z
                  << "\t" << p.vel.x << "\t" << p.vel.y << "\t" << p.vel.z
                  << "\t" << p.force.x << "\t" << p.force.y << "\t" << p.force.z;
    }
    std::cout << "\n";
}

// Function to initialize particles randomly
void random_init(std::vector<Particle>& particles, int n) {
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(-1e11, 1e11);
    std::uniform_real_distribution<double> vel(-1e3, 1e3);
    std::uniform_real_distribution<double> mass(1e20, 1e25);
    
    for (int i = 0; i < n; ++i) {
        Particle p;
        p.mass = mass(gen);
        p.pos = {dist(gen), dist(gen), dist(gen)};
        p.vel = {vel(gen), vel(gen), vel(gen)};
        p.force = {0, 0, 0};
        particles.push_back(p);
    }
}

// Function to initialize the solar system configuration
void solar_system_init(std::vector<Particle>& particles) {
    // Define the solar system with some basic particles
    particles.push_back({1.989e30, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}); // Sun
    particles.push_back({5.972e24, {1.496e11, 0, 0}, {0, 29780, 0}, {0, 0, 0}}); // Earth
    particles.push_back({7.348e22, {1.496e11 + 384400000, 0, 0}, {0, 29780 + 1022, 0}, {0, 0, 0}}); // Moon

    // Add more particles here as needed
}

// Main simulation loop
int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <num_particles|'planet'|filename> <dt> <steps> <output_interval>\n";
        return 1;
    }

    std::vector<Particle> particles;
    std::string mode = argv[1];
    double dt = std::stod(argv[2]);
    int steps = std::stoi(argv[3]);
    int output_interval = std::stoi(argv[4]);

    // Initialize particles based on the input mode
    if (mode == "planet") {
        solar_system_init(particles);
    } else if (isdigit(mode[0])) {
        int n = std::stoi(mode);
        random_init(particles, n);
    } else {
        std::cerr << "Unknown initialization mode: " << mode << std::endl;
        return 1;
    }

    // Start the clock for benchmarking
    auto start = std::chrono::high_resolution_clock::now();

    // Main simulation loop
    for (int step = 0; step <= steps; ++step) {
        compute_forces(particles);
        update_particles(particles, dt);
        if (step % output_interval == 0) output_state(particles);
    }

    // End the clock and print elapsed time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cerr << "Simulation took " << duration.count() << " seconds\n";

    return 0;
}
