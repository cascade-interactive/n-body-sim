First experiments 8/17/2026

Spect:
- CPU: Intel i7 14700k
- GPU: RTX 4070ti SUPER

Preset:
- 10000 steps
- 600 sec timestep
- 100 bodies

CPU:
Warmup: 503300 ns
Total time excluding warmup: 5853328200 ns
Average timestep excluding warmup: 585391 ns

CUDA:
Warmup: 93149800 ns
Total time excluding warmup: 22833333100 ns
Average timestep excluding warmup: 2.28356e+06 ns

Preset
- 5 steps
- 600 sec timestep
- 10000 bodies

CPU:
Warmup: 4719061500 ns
Total time excluding warmup: 18679091100 ns
Average timestep excluding warmup: 4.66977e+09 ns

CUDA:
Warmup: 109187200 ns
Total time excluding warmup: 594858900 ns
Average timestep excluding warmup: 1.48715e+08 ns

Release build: Almost tied at 10 steps, 100dt, 5000 bodies

Preset
- 10 steps
- 100 sec timestep
- 5000 bodies

CPU:
Warmup: 111792400 ns
Total time excluding warmup: 753606500 ns
Average timestep excluding warmup: 8.37341e+07 ns

CUDA:
Warmup: 115380700 ns
Total time excluding warmup: 74926200 ns
Average timestep excluding warmup: 8.32513e+06 ns
