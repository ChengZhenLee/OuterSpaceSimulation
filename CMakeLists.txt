cmake_minimum_required(VERSION 3.10)
project(OuterSpaceSimulation)

# Use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link the library files
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Include the source
add_executable(OuterSpaceSimulation src/main.cpp)
