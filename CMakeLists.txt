cmake_minimum_required(VERSION 3.20)

project(nbodysim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nbodysim 
	src/main.cpp
)

target_include_directories(nbodysim PRIVATE include)
target_compile_features(nbodysim PRIVATE cxx_std_20)
