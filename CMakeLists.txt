cmake_minimum_required(VERSION 3.10)
project(OuterSpaceSimulation)

# Use C++ 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/constants)

# Include the source
add_executable(OuterSpaceSimulation 
    src/main.cpp
    src/visuals/visuals.cpp)

# Link the library files
if(WIN32)
    target_link_libraries(OuterSpaceSimulation 
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylib.a"
        opengl32
        gdi32
        winmm
    )
else(APPLE)
    find_library(RAYLIB_LIB raylib)

    target_link_libraries(OuterSpaceSimulation 
        ${RAYLIB_LIB}
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework CoreVideo"
    )
endif()