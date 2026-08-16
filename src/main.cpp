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
  // for (int i = 0; i < argc; ++i) {
  //  std::cout << argv[i] << '\n';
  //}

  std::vector<Body> bodies;

  Body sun{.position = {0.0, 0.0, 0.0},
           .velocity = {0.0, 0.0, 0.0},
           .mass = 1.989e30,
           .radius = 696340e3};

  Body mercury{.position = {3.04599e10, 4.92358e10, 1.22637e9},
               .velocity = {-40486.4, 24903.8, 5750.44},
               .mass = 3.30104e23,
               .radius = 2439.7e3};

  Body venus{.position = {-9.02479e10, 5.93999e10, 6.02163e9},
             .velocity = {-19219.7, -29273.0, 709.419},
             .mass = 4.86732e24,
             .radius = 6051.8e3};

  Body earth{.position = {-1.14599e11, 9.61599e10, 8.39154e4},
             .velocity = {-19148.1, -22819.8, -0.0199},
             .mass = 5.97219e24,
             .radius = 6371e3};

  Body moon{.position = {-1.14327e11, 9.64306e10, 2.44591e7},
            .velocity = {-19872.5, -22098.2, 64.9477},
            .mass = 7.34767e22,
            .radius = 1737.4e3};

  Body mars{.position = {-4.13499e10, -2.24132e11, -3.67935e9},
            .velocity = {23724.9, -4365.92, -674.700},
            .mass = 6.41693e23,
            .radius = 3389.5e3};

  Body jupiter{.position = {7.28998e11, 2.72163e11, -1.74437e10},
               .velocity = {-4568.82, 12241.1, 51.6333},
               .mass = 1.89813e27,
               .radius = 69911e3};

  Body saturn{.position = {1.56927e11, 1.41767e12, -3.09908e10},
              .velocity = {-9581.41, 1068.54, 362.988},
              .mass = 5.68319e26,
              .radius = 58232e3};

  Body uranus{.position = {-1.16836e12, 2.62202e12, 2.47973e10},
              .velocity = {-6211.12, -2768.30, 70.0080},
              .mass = 8.68103e25,
              .radius = 25362e3};

  Body neptune{.position = {-2.78150e12, -3.53272e12, 1.36833e11},
               .velocity = {4267.69, -3361.32, -29.1376},
               .mass = 1.02410e26,
               .radius = 24622e3};

  bodies.push_back(sun);
  bodies.push_back(mercury);
  bodies.push_back(venus);
  bodies.push_back(earth);
  bodies.push_back(moon);
  bodies.push_back(mars);
  bodies.push_back(jupiter);
  bodies.push_back(saturn);
  bodies.push_back(uranus);
  bodies.push_back(neptune);

  // ArgV is the timestep
  // Update the position of the earth based on its velocity and the timestep

  std::cout << "Number of steps: " << argv[1] << '\n';
  std::cout << "Timestep: " << argv[2] << '\n';

  for (int i = 0; i < std::stoi(argv[1]); ++i) {

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

      // Output the position, velocity, and acceleration of the j-th body to the
      // output file
      output_file << "Step " << j << ":\n";
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

  // Close the output file
  output_file.close();

  return 0;
}
