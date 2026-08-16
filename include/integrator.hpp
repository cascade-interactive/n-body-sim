#pragma once

#include "body.hpp"
#include "simMath.hpp"
#include <vector>


inline constexpr double G = 6.67430e-11; // Gravitational constant

Vector3 calculate_acceleration(std::size_t i, const std::vector<Body> &bodies,
                               const std::vector<Vector3> &positions);
void integrate_verlet(std::vector<Body> &bodies, double dt);