// Alexander Bugar
// Main entry point

#include "../include/body.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv) {

  for (int i = 0; i < argc; ++i)
    std::cout << argv[i] << '\n';

  Body earth{.position = {149.6e9, 0.0, 0.0},
             .velocity = {0.0, 29780.0, 0.0},
             .mass = 5.972e24,
             .radius = 6371e3};

  Body sun{.position = {0.0, 0.0, 0.0},
           .velocity = {0.0, 0.0, 0.0},
           .mass = 1.989e30,
           .radius = 696340e3};

  for (int i = 0; i < std::stoi(argv[1]); ++i) {

    // ArgV is the timestep
    // Update the position of the earth based on its velocity and the timestep
    int dt = 10;

    Vector3 r = sun.position - earth.position;
    double distance = Length(r);

    // Calculate the gravitational force between the earth and the sun
    double G = 6.67430e-11; // Gravitational constant

    double scale = G * sun.mass / (distance * distance * distance);

    earth.acceleration.x = r.x * scale;
    earth.acceleration.y = r.y * scale;
    earth.acceleration.z = r.z * scale;

    earth.velocity.x += earth.acceleration.x * dt;
    earth.velocity.y += earth.acceleration.y * dt;
    earth.velocity.z += earth.acceleration.z * dt;

    earth.position.x += earth.velocity.x * dt;
    earth.position.y += earth.velocity.y * dt;
    earth.position.z += earth.velocity.z * dt;

    std::cout << "Earth position: (" << earth.position.x << ", "
              << earth.position.y << ", " << earth.position.z << ")\n";
    std::cout << "Earth velocity: (" << earth.velocity.x << ", "
              << earth.velocity.y << ", " << earth.velocity.z << ")\n";
    std::cout << "Earth acceleration: (" << earth.acceleration.x << ", "
              << earth.acceleration.y << ", " << earth.acceleration.z << ")\n";
  }

  return 0;
}
