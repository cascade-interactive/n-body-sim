#pragma once
void cuda();
void cuda_integrate_verlet(std::vector<Body> &bodies, double dt);
inline constexpr double G = 6.67430e-11; // Gravitational constant
