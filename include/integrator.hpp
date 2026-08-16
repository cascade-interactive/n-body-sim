#pragma once

#include "simMath.hpp"

void calculate_acceleration(Vector3 position, Vector3* acceleration);
void integrate_verlet(Vector3* position, Vector3* velocity, Vector3* acceleration, double dt);
