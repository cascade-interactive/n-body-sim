// Alexander Bugar
// Main entry point

#include "../include/body.hpp"
#include "../include/integrator.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {

  std::int64_t total_time_ns = 0;

  // Check file
  std::ofstream output_file("output.txt");
  if (!output_file.is_open()) {
    std::cerr << "Error: Could not open output file.\n";
    return 1;
  }

  // Check command line arguments
  // for (int i = 0; i < argc; ++i) {
  //  std::cout << argv[i] << '\n';
  //}

  std::vector<Body> bodies;

  Body sun{.position = {0.0, 0.0, 0.0},
           .velocity = {0.0, 0.0, 0.0},
           .mass = 1.989e30,
           .radius = 696340e3};

  // ~1.5 Jupiter masses
  // Starts near 1 AU, prograde, significantly inclined.
  Body planet1{.position = {1.496e11, 0.0, 0.0},
               .velocity = {0.0, 27000.0, 8500.0},
               .mass = 2.85e27,
               .radius = 75000e3};

  // ~2 Jupiter masses
  // Close enough to planet1 for substantial perturbations.
  Body planet2{.position = {1.72e11, 3.0e10, 1.5e10},
               .velocity = {-7000.0, 28500.0, -6000.0},
               .mass = 3.80e27,
               .radius = 80000e3};

  // ~1 Jupiter mass
  // Retrograde orbit. This makes encounters much more violent.
  Body planet3{.position = {-2.05e11, 2.5e10, -2.0e10},
               .velocity = {3000.0, -24500.0, 7500.0},
               .mass = 1.90e27,
               .radius = 70000e3};

  // ~3 Jupiter masses
  // Outer perturber with a strongly tilted velocity vector.
  Body planet4{.position = {2.0e10, -2.75e11, 4.0e10},
               .velocity = {20500.0, 3500.0, -5500.0},
               .mass = 5.70e27,
               .radius = 90000e3};

  bodies.push_back(sun);
  bodies.push_back(planet1);
  bodies.push_back(planet2);
  bodies.push_back(planet3);
  bodies.push_back(planet4);

  // ArgV is the timestep
  // Update the position of the earth based on its velocity and the timestep

  std::cout << "Number of steps: " << argv[1] << '\n';
  std::cout << "Timestep: " << argv[2] << '\n';

  int steps = std::stoi(argv[1]);
  double dt = std::stod(argv[2]);

  for (int i = 0; i < steps; ++i) {
    auto tick_start = std::chrono::high_resolution_clock::now();

    integrate_verlet(bodies, dt);

    auto tick_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        tick_end - tick_start);
    total_time_ns += elapsed.count();

    std::cout << "Step " << i << " completed in " << elapsed.count() << " ns\n";

    // Output file logging
    output_file << "Step " << i << ":\n";
    for (std::size_t j = 0; j < bodies.size(); ++j) {
      output_file << "Body " << j << ": Position(" << bodies[j].position.x
                  << ", " << bodies[j].position.y << ", "
                  << bodies[j].position.z << "), "
                  << "Velocity(" << bodies[j].velocity.x << ", "
                  << bodies[j].velocity.y << ", " << bodies[j].velocity.z
                  << "), "
                  << "Acceleration(" << bodies[j].acceleration.x << ", "
                  << bodies[j].acceleration.y << ", "
                  << bodies[j].acceleration.z << ")\n";
    }
    output_file << "\n";
  }

  // Close the output file
  output_file.close();

  double average_time_ns = static_cast<double>(total_time_ns) / steps;
  std::cout << "Average time per step: " << average_time_ns << " ns\n";

  return 0;
}
