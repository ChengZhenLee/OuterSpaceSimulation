cmake_minimum_required(VERSION 3.10)
project(OuterSpaceSimulation)

# Use C++ 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the source
add_executable(OuterSpaceSimulation 
    src/manager/inputmanager.cpp
    src/visuals/UIComponent.cpp
    src/visuals/Renderer.cpp
    src/physics/physics.cpp
    src/physics/simulation.cpp
    src/app.cpp
    src/main.cpp
)

# Include the headers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/constants
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link the library files
if(WIN32)
    target_link_libraries(OuterSpaceSimulation 
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylib.a"
        opengl32
        gdi32
        winmm
    )
else(APPLE)
    find_library(RAYLIB_LIB raylib)

    target_link_libraries(OuterSpaceSimulation 
        PRIVATE
        ${RAYLIB_LIB}
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework CoreVideo"
    )
endif()