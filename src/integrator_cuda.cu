#include <iostream>
#include <cuda_runtime.h>
#include <vector>
#include "simMath.hpp"
#include "body.hpp"
#include "integrator_cuda.hpp"

inline constexpr double G = 6.67430e-11;

CudaIntegrator::CudaIntegrator(double dt) : dt_(dt) {
  init();
}

void CudaIntegrator::init() {
  // Initialize CUDA
  cudaSetDevice(0);
}

void CudaIntegrator::shutdown() {
  cudaDeviceReset();
}

void CudaIntegrator::integrate_verlet(std::vector<Body> &bodies, double dt) {
  int n = static_cast<int>(bodies.size());
  
  if (n == 0) return;

  int threads = 256;
  int blocks = (n + threads - 1) / threads;

  Body *d_bodies = nullptr;
  Vector3 *d_new_accelerations = nullptr;

  cudaMalloc(reinterpret_cast<void **>(&d_bodies), n * sizeof(Body));
  cudaMalloc(reinterpret_cast<void **>(&d_new_accelerations), n * sizeof(Vector3));

  cudaMemcpy(d_bodies, bodies.data(), n * sizeof(Body), cudaMemcpyHostToDevice);

  update_positions<<<blocks, threads>>>(d_bodies, n, dt);
  calculate_acceleration<<<blocks, threads>>>(d_bodies, d_new_accelerations, n);
  update_velocities<<<blocks, threads>>>(d_bodies, d_new_accelerations, n, dt);

  cudaDeviceSynchronize();

  cudaMemcpy(bodies.data(), d_bodies, n * sizeof(Body), cudaMemcpyDeviceToHost);

  cudaFree(d_new_accelerations);
  cudaFree(d_bodies);
}

__global__ void update_positions(Body *bodies, int n, double dt) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  if (i >= n) return;

  bodies[i].position.x = bodies[i].position.x + bodies[i].velocity.x * dt + 0.5 * bodies[i].acceleration.x * dt * dt;
  bodies[i].position.y = bodies[i].position.y + bodies[i].velocity.y * dt + 0.5 * bodies[i].acceleration.y * dt * dt;
  bodies[i].position.z = bodies[i].position.z + bodies[i].velocity.z * dt + 0.5 * bodies[i].acceleration.z * dt * dt;

}

__global__ void calculate_acceleration(Body *bodies, Vector3 *new_accelerations, int n) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  if (i >= n) return;

  double ax = 0.0;
  double ay = 0.0;
  double az = 0.0;

  Vector3 displacement;
  double distance;
  double factor;

  for (int j = 0; j < n; ++j) {
    if (i == j) continue;

    displacement.x = bodies[j].position.x - bodies[i].position.x;
    displacement.y = bodies[j].position.y - bodies[i].position.y;
    displacement.z = bodies[j].position.z - bodies[i].position.z;

    distance = sqrt(displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z);

    factor = G * bodies[j].mass / (distance * distance * distance);
    
    ax += factor * displacement.x;
    ay += factor * displacement.y;
    az += factor * displacement.z;

  }

  new_accelerations[i].x = ax;
  new_accelerations[i].y = ay;
  new_accelerations[i].z = az;
}

__global__ void update_velocities(Body *bodies, Vector3 *new_accelerations, int n, double dt) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  if (i >= n) return;

  bodies[i].velocity.x = bodies[i].velocity.x + 0.5 * (bodies[i].acceleration.x + new_accelerations[i].x) * dt;
  bodies[i].velocity.y = bodies[i].velocity.y + 0.5 * (bodies[i].acceleration.y + new_accelerations[i].y) * dt;
  bodies[i].velocity.z = bodies[i].velocity.z + 0.5 * (bodies[i].acceleration.z + new_accelerations[i].z) * dt;
  
  bodies[i].acceleration = new_accelerations[i];
}