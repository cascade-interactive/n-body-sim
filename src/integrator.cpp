// integrator.cpp
#include "integrator.hpp"

void integrate_verlet(Vector3* position, Vector3* velocity, Vector3* acceleration, double dt) {
  Vector3 previous_position = *position;
  Vector3 previous_velocity = *velocity;
  Vector3 previous_acceleration = *acceleration;

  Vector3 new_position = *position + *velocity * dt + 0.5 * (*acceleration) * dt * dt;
  Vector3 new_acceleration;
  
  calculate_acceleration(new_position, &new_acceleration);
  Vector3 new_velocity = *velocity + 0.5 * (*acceleration + new_acceleration) * dt;

  *position = new_position;
  *velocity = new_velocity;
  *acceleration = new_acceleration;
}

void calculate_acceleration(Vector3 position, Vector3* acceleration) {
  // Calculate the gravitational force between the earth and the sun
  double G = 6.67430e-11; // Gravitational constant
  Vector3 sun_position = {0.0, 0.0, 0.0};
  double sun_mass = 1.989e30;

  Vector3 r = sun_position - position;
  double distance = Length(r);

  double scale = G * sun_mass / (distance * distance * distance);

  acceleration->x = r.x * scale;
  acceleration->y = r.y * scale;
  acceleration->z = r.z * scale;
}