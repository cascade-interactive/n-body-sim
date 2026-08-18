#include "simulation.hpp"

static std::int64_t cpu_total_time_ns = 0;
static std::int64_t cuda_total_time_ns = 0;

static std::int64_t cpu_warmup_time_ns = 0;
static std::int64_t cuda_warmup_time_ns = 0;
static Simulation::IntegratorMode active_mode =
  Simulation::IntegratorMode::None;

Simulation::Simulation(int steps, double dt, int num_bodies) {
  steps_ = steps;
  num_bodies_ = num_bodies;
  dt_ = dt;
}

// Runs once on init
void Simulation::begin(IntegratorMode mode) {

  active_mode = mode;

  std::cout << "Number of steps: " << steps_ << '\n';
  std::cout << "Timestep: " << dt_ << '\n';

  // Check file
  output_file_.open("output.txt");
  if (!output_file_.is_open()) {
    throw std::runtime_error("Error: Could not open output file.");
    return;
  }

  std::cout << "Beginning simulation" << "\n";

  Body sun{.position = {0.0, 0.0, 0.0},
           .velocity = {0.0, 0.0, 0.0},
           .mass = 1.989e30,
           .radius = 696340e3};

  bodies.push_back(sun);

  // Randomly generate position, velocity, mass, and radius for each body
  std::mt19937 gen(42);

  std::uniform_real_distribution<> pos_dist(-1e11, 1e11);
  std::uniform_real_distribution<> vel_dist(-1e4, 1e4);
  std::uniform_real_distribution<> mass_dist(1e20, 1e30);
  std::uniform_real_distribution<> radius_dist(1e3, 1e7);

  for (int i = 1; i < num_bodies_; ++i) {

    Body body{
        .position = {pos_dist(gen), pos_dist(gen), pos_dist(gen)},
        .velocity = {vel_dist(gen), vel_dist(gen), vel_dist(gen)},
        .mass = mass_dist(gen),
        .radius = radius_dist(gen),
    };

    bodies.push_back(body);
  }

  // Initialize accelerations
  std::vector<Vector3> positions;
  positions.reserve(bodies.size());

  for (const Body &body : bodies) {
    positions.push_back(body.position);
  }

  for (std::size_t i = 0; i < bodies.size(); ++i) {
    bodies[i].acceleration = calculate_acceleration(i, bodies, positions);
  }
}

// Runs every tick
void Simulation::update(uint64_t iteration) {
  int i = iteration;

  std::vector<Body> cpu_bodies = bodies;
  std::vector<Body> cuda_bodies = bodies;

  std::chrono::nanoseconds cpu_elapsed{0};
  std::chrono::nanoseconds cuda_elapsed{0};

  if (active_mode == IntegratorMode::Cpu ||
      active_mode == IntegratorMode::Both) {
    auto cpu_tick_start = std::chrono::high_resolution_clock::now();
    integrate_verlet(cpu_bodies, dt_);
    auto cpu_tick_end = std::chrono::high_resolution_clock::now();

    cpu_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        cpu_tick_end - cpu_tick_start);

    if (iteration == 0) {
      cpu_warmup_time_ns = cpu_elapsed.count();
    } else {
      cpu_total_time_ns += cpu_elapsed.count();
    }
  }

  if (active_mode == IntegratorMode::Gpu ||
      active_mode == IntegratorMode::Both) {
    auto cuda_tick_start = std::chrono::high_resolution_clock::now();
    CudaIntegrator::integrate_verlet(cuda_bodies, dt_);
    auto cuda_tick_end = std::chrono::high_resolution_clock::now();

    cuda_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        cuda_tick_end - cuda_tick_start);

    if (iteration == 0) {
      cuda_warmup_time_ns = cuda_elapsed.count();
    } else {
      cuda_total_time_ns += cuda_elapsed.count();
    }
  }

  std::cout << "Step " << i;
  if (active_mode == IntegratorMode::Cpu ||
      active_mode == IntegratorMode::Both) {
    std::cout << " CPU: " << cpu_elapsed.count() << " ns";
  }
  if (active_mode == IntegratorMode::Gpu ||
      active_mode == IntegratorMode::Both) {
    std::cout << " CUDA: " << cuda_elapsed.count() << " ns";
  }
  std::cout << "\n";

  if (active_mode == IntegratorMode::Cpu) {
    bodies = cpu_bodies;
  } else if (active_mode == IntegratorMode::Gpu ||
             active_mode == IntegratorMode::Both) {
    bodies = cuda_bodies;
  }

  // Output file logging
  output_file_ << "Step " << i << ":\n";

  for (std::size_t j = 0; j < bodies.size(); ++j) {
    output_file_ << "Body " << j << ": Position(" << bodies[j].position.x
                 << ", " << bodies[j].position.y << ", " << bodies[j].position.z
                 << "), "
                 << "Velocity(" << bodies[j].velocity.x << ", "
                 << bodies[j].velocity.y << ", " << bodies[j].velocity.z
                 << "), "
                 << "Acceleration(" << bodies[j].acceleration.x << ", "
                 << bodies[j].acceleration.y << ", " << bodies[j].acceleration.z
                 << ")\n";
  }

  output_file_ << "\n";
}

void Simulation::terminate() {
  // Close the output file
  output_file_.close();

  int measured_steps = steps_ - 1;

  double cpu_average_time_ns =
      static_cast<double>(cpu_total_time_ns) / measured_steps;

  double cuda_average_time_ns =
      static_cast<double>(cuda_total_time_ns) / measured_steps;

  std::cout << "\nCPU:\n";
  std::cout << "Warmup: " << cpu_warmup_time_ns << " ns\n";
  std::cout << "Total time excluding warmup: " << cpu_total_time_ns << " ns\n";
  std::cout << "Average timestep excluding warmup: " << cpu_average_time_ns
            << " ns\n";

  std::cout << "\nCUDA:\n";
  std::cout << "Warmup: " << cuda_warmup_time_ns << " ns\n";
  std::cout << "Total time excluding warmup: " << cuda_total_time_ns << " ns\n";
  std::cout << "Average timestep excluding warmup: " << cuda_average_time_ns
            << " ns\n";
}