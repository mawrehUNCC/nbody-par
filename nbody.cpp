#include "nbody.hpp"

void NBodySimulation::initRandom(int numParticles) {
    particles.clear();
    particles.resize(numParticles);

    for (auto& p : particles) {
        p.mass = (rand() % 100 + 1) * 1.0e25;
        p.position = {static_cast<double>(rand() % 1000), static_cast<double>(rand() % 1000), static_cast<double>(rand() % 1000)};
        p.velocity = {static_cast<double>(rand() % 10), static_cast<double>(rand() % 10), static_cast<double>(rand() % 10)};
        p.force = {0, 0, 0};
    }
}

void NBodySimulation::initSolarSystem() {
    // Solar system initialization code here (e.g., Sun, planets, etc.)
}

void NBodySimulation::resetForces() {
    for (auto& p : particles) {
        p.force = {0, 0, 0};
    }
}

void NBodySimulation::computeForcesParallel() {
    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            Vec3 dir = particles[j].position - particles[i].position;
            double dist = dir.norm() + softening;
            double forceMagnitude = (particles[i].mass * particles[j].mass) / (dist * dist);
            dir = dir * (forceMagnitude / dist);

            particles[i].force += dir;
            particles[j].force -= dir;
        }
    }
}

void NBodySimulation::integrateMotionParallel() {
    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); ++i) {
        particles[i].velocity += particles[i].force * dt / particles[i].mass;
        particles[i].position += particles[i].velocity * dt;
    }
}

void NBodySimulation::outputState(std::ostream& out) {
    out << particles.size();
    for (const auto& p : particles) {
        out << "\t" << p.mass
            << "\t" << p.position.x << "\t" << p.position.y << "\t" << p.position.z
            << "\t" << p.velocity.x << "\t" << p.velocity.y << "\t" << p.velocity.z
            << "\t" << p.force.x << "\t" << p.force.y << "\t" << p.force.z;
    }
    out << "\n";
}
