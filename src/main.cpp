// Alexander Bugar
// Main entry point

#include "../include/body.hpp"
#include "../include/integrator.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv) {

  std::ofstream output_file("output.txt");
  if (!output_file.is_open()) {
    std::cerr << "Error: Could not open output file.\n";
    return 1;
  }

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

    integrate_verlet(&earth.position, &earth.velocity, &earth.acceleration, std::stod(argv[2]));
    if (i % 1000 == 0) 
    {
    output_file << "Time: " << i * std::stod(argv[2]) << " seconds\n";
    output_file << "Earth Position: (" << earth.position.x << ", " << earth.position.y << ", " << earth.position.z << std::endl;
    output_file << "Earth Velocity: (" << earth.velocity.x << ", " << earth.velocity.y << ", " << earth.velocity.z << std::endl;
    output_file << "Earth Acceleration: (" << earth.acceleration.x << ", " << earth.acceleration.y << ", " << earth.acceleration.z << std::endl;
    output_file << "\n";
    }
  }

  output_file.close();

  return 0;
}
