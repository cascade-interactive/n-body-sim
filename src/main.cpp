// Alexander Bugar
// Main entry point

#include "../include/body.hpp"
#include "../include/integrator.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char **argv) {

  // Check file
  std::ofstream output_file("output.txt");
  if (!output_file.is_open()) {
    std::cerr << "Error: Could not open output file.\n";
    return 1;
  }

  // Check command line arguments
  for (int i = 0; i < argc; ++i)
    std::cout << argv[i] << '\n';

  std::vector<Body> bodies;

  Body earth{.position = {149.6e9, 0.0, 0.0},
             .velocity = {0.0, 29780.0, 0.0},
             .mass = 5.972e24,
             .radius = 6371e3};

  Body sun{.position = {0.0, 0.0, 0.0},
           .velocity = {0.0, 0.0, 0.0},
           .mass = 1.989e30,
           .radius = 696340e3};

  Body moon{.position = {149.6e9 + 384.4e6, 0.0, 0.0},
            .velocity = {0.0, 29780.0 + 1022.0, 0.0},
            .mass = 7.348e22,
            .radius = 1737e3};

  bodies.push_back(sun);
  bodies.push_back(earth);
  bodies.push_back(moon);

  for (int i = 0; i < std::stoi(argv[1]); ++i) {

    // ArgV is the timestep
    // Update the position of the earth based on its velocity and the timestep

    for (std::size_t j = 0; j < bodies.size(); ++j) {
      // Work on the j-th body
      for (std::size_t k = 0; k < bodies.size(); ++k) {
        if (j != k) {
          // Calculate the acceleration of the j-th body due to the k-th body
          Body &body_j = bodies[j];
          Body &body_k = bodies[k];

          // void integrate_verlet(std::vector<Body>& bodies, double dt)
          integrate_verlet(bodies, std::stod(argv[2]));
        }
      }
    }

    if (i % 1000 == 0) {
      output_file << "Step " << i << ":\n";
      for (std::size_t j = 0; j < bodies.size(); ++j) {
        output_file << "Body " << j << ": Position(" << bodies[j].position.x
                    << ", " << bodies[j].position.y << ", "
                    << bodies[j].position.z << "), "
                    << "Velocity(" << bodies[j].velocity.x << ", "
                    << bodies[j].velocity.y << ", " << bodies[j].velocity.z
                    << "), "
                    << "Acceleration(" << bodies[j].acceleration.x << ", "
                    << bodies[j].acceleration.y << ", "
                    << bodies[j].acceleration.z << ")\n";
      }
      output_file << "\n";
    }
  }

  output_file.close();

  return 0;
}
