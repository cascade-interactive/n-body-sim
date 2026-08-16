cmake_minimum_required(VERSION 3.20)

project(nbodysim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nbodysim 
	src/main.cpp
)
# include the include directory for header files and integrator.cpp from /src
target_sources(nbodysim PRIVATE src/integrator.cpp)
target_include_directories(nbodysim PRIVATE include)
target_compile_features(nbodysim PRIVATE cxx_std_20)
