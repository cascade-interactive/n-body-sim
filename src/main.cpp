// Alexander Bugar
// Main entry point

#include "../include/body.hpp"
#include "../include/integrator.hpp"
#include "../include/simulation.hpp"
#include "integrator_cuda.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>



Simulation::IntegratorMode mode = Simulation::IntegratorMode::None;

int main(int argc, char **argv) {

  // Check command line arguments
  if (argc < 4 || argc > 5) {
    std::cerr << "Usage: " << argv[0]
              << " <number_of_steps> <timestep> <bodies>\n";
    return 1;
  }

  if (argc == 5) {
    std::string flag = argv[4];

    if (flag == "--c" || flag == "cpu") mode = Simulation::IntegratorMode::Cpu;
    else if (flag == "--g" || flag == "gpu") mode = Simulation::IntegratorMode::Gpu;
    else if(flag == "--b" || flag == "both") mode = Simulation::IntegratorMode::Both;
    else {
      std::cerr << "Unknown integrator: " << flag << '\n';
      return 1;
    }
  }

  const int steps = std::stoi(argv[1]);
  const double dt = std::stod(argv[2]);
  const int num_bodies = std::stoi(argv[3]);

  if (num_bodies < 2) {
    std::cerr << "Need at least 2 bodies.\n";
    return 1;
  }

  Simulation sim(steps, dt, num_bodies);
  CudaIntegrator cu_integrator(dt);

  sim.begin(mode);

  for (int i = 0; i < steps; ++i) {
    sim.update((uint64_t)i);
  }

  sim.terminate();
  cu_integrator.shutdown();

  return 0;
}
