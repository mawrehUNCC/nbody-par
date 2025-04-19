#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <omp.h>
#include <cstring>

const double G = 6.674e-11;
const double SOFTENING = 1e-9;

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

struct Particle {
    double mass;
    Vec3 pos, vel, force;
};

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

void update_particles(std::vector<Particle>& particles, double dt) {
    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); ++i) {
        Vec3 acc = particles[i].force / particles[i].mass;
        particles[i].vel += acc * dt;
        particles[i].pos += particles[i].vel * dt;
    }
}

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

    if (isdigit(mode[0])) {
        int n = std::stoi(mode);
        random_init(particles, n);
    } else {
        std::cerr << "Only random init supported for now.\n";
        return 1;
    }

    for (int step = 0; step <= steps; ++step) {
        compute_forces(particles);
        update_particles(particles, dt);
        if (step % output_interval == 0) output_state(particles);
    }

    return 0;
}
