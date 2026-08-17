#include "simulation.hpp"

Simulation::Simulation(int steps, double dt, int num_bodies) {
  steps_ = steps;
  num_bodies_ = num_bodies;
  dt_ = dt;
}

// Runs once on init
void Simulation::begin() {

  std::cout << "Number of steps: " << steps_ << '\n';
  std::cout << "Timestep: " << dt_ << '\n';

  // Check file
  output_file_.open("output.txt");
  if (!output_file_.is_open()) {
    throw std::runtime_error("Error: Could not open output file.");
    return;
  }

  std::cout << "Beginning simulation" << "\n";

  Body sun{.position = {0.0, 0.0, 0.0},
           .velocity = {0.0, 0.0, 0.0},
           .mass = 1.989e30,
           .radius = 696340e3};

  bodies.push_back(sun);

  for (int i = 1; i < num_bodies_; ++i) {
    // Randomly generate position, velocity, mass, and radius for each body
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> pos_dist(-1e11, 1e11);
    std::uniform_real_distribution<> vel_dist(-1e4, 1e4);
    std::uniform_real_distribution<> mass_dist(1e20, 1e30);
    std::uniform_real_distribution<> radius_dist(1e3, 1e7);

    Body body{
        .position = {pos_dist(gen), pos_dist(gen), pos_dist(gen)},
        .velocity = {vel_dist(gen), vel_dist(gen), vel_dist(gen)},
        .mass = mass_dist(gen),
        .radius = radius_dist(gen),
    };

    bodies.push_back(body);
  }
}

// Runs every tick
void Simulation::update(uint64_t iteration) {
  int i = iteration;
  auto tick_start = std::chrono::high_resolution_clock::now();

  cuda_integrate_verlet(bodies, dt_);

  auto tick_end = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
      tick_end - tick_start);
  total_time_ns += elapsed.count();

  std::cout << "Step " << i << " completed in " << elapsed.count() << "ns\n";

  // Output file logging
  output_file_ << "Step " << i << ":\n";
  for (std::size_t j = 0; j < bodies.size(); ++j) {
    output_file_ << "Body " << j << ": Position(" << bodies[j].position.x
                 << ", " << bodies[j].position.y << ", " << bodies[j].position.z
                 << "), "
                 << "Velocity(" << bodies[j].velocity.x << ", "
                 << bodies[j].velocity.y << ", " << bodies[j].velocity.z
                 << "), "
                 << "Acceleration(" << bodies[j].acceleration.x << ", "
                 << bodies[j].acceleration.y << ", " << bodies[j].acceleration.z
                 << ")\n";
  }
  output_file_ << "\n";
}

void Simulation::terminate() {
  // Close the output file
  output_file_.close();

  double average_time_ns = static_cast<double>(total_time_ns) / steps_;
  std::cout << "Average time per step: " << average_time_ns << " ns\n";
}
