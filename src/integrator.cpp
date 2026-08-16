// integrator.cpp
#include "integrator.hpp"

void integrate_verlet(std::vector<Body> &bodies, double dt) {
  std::vector<Vector3> new_positions(bodies.size());
  std::vector<Vector3> new_accelerations(bodies.size());
  std::vector<Vector3> new_velocities(bodies.size());

  for (std::size_t i = 0; i < bodies.size(); ++i) {
    new_positions[i] = bodies[i].position + bodies[i].velocity * dt +
                       0.5 * bodies[i].acceleration * dt * dt;
  }

  for (std::size_t i = 0; i < bodies.size(); ++i) {
    new_accelerations[i] = calculate_acceleration(i, bodies, new_positions);
  }

  for (std::size_t i = 0; i < bodies.size(); ++i) {
    new_velocities[i] =
        bodies[i].velocity +
        0.5 * (bodies[i].acceleration + new_accelerations[i]) * dt;
  }

  for (std::size_t i = 0; i < bodies.size(); ++i) {
    bodies[i].position = new_positions[i];
    bodies[i].velocity = new_velocities[i];
    bodies[i].acceleration = new_accelerations[i];
  }

  // Vector3 previous_position = *position;
  // Vector3 previous_velocity = *velocity;
  // Vector3 previous_acceleration = *acceleration;

  // Vector3 new_position = *position + *velocity * dt + 0.5 * (*acceleration) *
  // dt * dt; Vector3 new_acceleration;
  //
  // calculate_acceleration(new_position, &new_acceleration);
  // Vector3 new_velocity = *velocity + 0.5 * (*acceleration + new_acceleration)
  // * dt;

  // *position = new_position;
  // *velocity = new_velocity;
  // *acceleration = new_acceleration;
}

Vector3 calculate_acceleration(std::size_t i, const std::vector<Body> &bodies,
                               const std::vector<Vector3> &positions) {
  Vector3 total_acceleration{0.0, 0.0, 0.0};
  Vector3 displacement;
  double distance;
  Vector3 contribution;

  for (std::size_t j = 0; j < bodies.size(); ++j) {
    if (i != j) {
      displacement = positions[j] - positions[i];
      distance = Length(displacement);
      contribution =
          G * bodies[j].mass * displacement / (distance * distance * distance);
      total_acceleration += contribution;
    }
  }

  return total_acceleration;
}