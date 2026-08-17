#pragma once

#include "body.hpp"
#include "integrator.hpp"
#include "integrator_cuda.hpp"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

class Simulation {
public:
  Simulation(int steps, double dt, int num_bodies);
  void begin();
  void update(std::uint64_t iteration);
  void terminate();

private:
  void initialize();
  int steps_;
  double dt_;
  double time_;
  std::uint64_t iteration_;
  int num_bodies_;
  std::vector<Body> bodies;
  std::int64_t total_time_ns = 0;
  std::ofstream output_file_;
};