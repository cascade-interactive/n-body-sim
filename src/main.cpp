// Alexander Bugar
// Main entry point

#include "../include/body.hpp"
#include "../include/integrator.hpp"
#include "../include/simulation.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main(int argc, char **argv) {

  // Check command line arguments
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0]
              << " <number_of_steps> <timestep> <bodies>\n";
    return 1;
  }

  const int steps = std::stoi(argv[1]);
  const double dt = std::stod(argv[2]);
  const int num_bodies = std::stoi(argv[3]);

  if (num_bodies < 2) {
    std::cerr << "Need at least 2 bodies.\n";
    return 1;
  }

  Simulation sim(steps, dt, num_bodies);

  sim.begin();

  // ArgV is the timestep
  // Update the position of the earth based on its velocity and the timestep

  std::cout << "Number of steps: " << argv[1] << '\n';
  std::cout << "Timestep: " << argv[2] << '\n';

  for (int i = 0; i < steps; ++i) {
    sim.update((uint64_t)i);
  }

  sim.terminate();

  return 0;
}
