#pragma once

#include "body.hpp"
#include <vector>

class CudaIntegrator {
public:
  CudaIntegrator(double dt);
  static void integrate_verlet(std::vector<Body> &bodies, double dt);
  static void shutdown();

private:
  static void init();
  double dt_;
};

#ifdef __CUDACC__
__global__ void update_positions(Body*, int, double);
__global__ void calculate_acceleration(Body*, Vector3*, int);
__global__ void update_velocities(Body*, Vector3*, int, double);
#endif
