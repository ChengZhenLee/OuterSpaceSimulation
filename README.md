# Outer Space Simulation

A real-time 3D physics simulation of celestial bodies in outer space, built with C++ using Raylib for graphics rendering and Eigen and Algorithmic Differentiation for mathematical computations.

## Features

- **Real-time Physics Simulation**: Simulate gravitational interactions between celestial bodies with accurate physics calculations.
- **Collision Detection**: Handle collisions between bodies, including merging and explosive impacts.
- **3D Rendering**: Visualize the simulation in 3D space with customizable camera controls.
- **Interactive UI**: Add, remove, and modify celestial bodies through an intuitive user interface.
- **Explosion Effects**: Realistic particle-based explosions when bodies collide at high speeds.
- **Performance Monitoring**: Display FPS and real-time delta time for performance tracking.

## Dependencies

- **Raylib**: A simple and easy-to-use library to enjoy videogames programming. Used for window management, input handling, and 3D rendering.
- **Eigen**: A C++ template library for linear algebra. Used for vector and matrix operations in physics calculations.
- **AD**: An Algrithmic Differentiation tool developed by Professor Uwe Naumann to efficiently calculate derivatives.
- **CMake**: Build system generator.

## Building the Project

### Prerequisites

- C++20 compatible compiler (e.g., GCC, Clang, MSVC)
- CMake 3.10 or higher
- Raylib library (pre-built library included in `lib/` for Windows; for other platforms, install via package manager)

### Build Steps

1. Clone or download the project to your local machine.

2. Navigate to the project root directory.

3. Create a build directory:
   ```
   mkdir build
   cd build
   ```

4. Generate build files with CMake:
   ```
   cmake ..
   ```

5. Build the project:
   ```
   make
   ```
   (On Windows, use `cmake --build .` or open the generated solution in Visual Studio)

6. Run the executable:
   ```
   ./OuterSpaceSimulation
   ```

## Usage

You can run the simulation in two ways:

### Option 1: Run the Pre-built Executable (Windows Only)
A pre-built executable (`OuterSpaceSimulation.exe`) is included in the root directory for convenience. Simply double-click it or run it from the command line to start the simulation immediately without building.

### Option 2: Build from Source
Follow the build steps above to compile the project yourself. This is recommended if you're on a different platform or want to modify the code.

Once running, you can interact with the simulation using the following controls:

- **Mouse Left Click** (outside UI): Enable free camera movement
- **WASD**: Move around the simulation while in free camera mode
- **ESC**: Disable camera movement and show cursor
- **F**: Toggle fullscreen mode
- **E**: Pause/Resume simulation
- **C**: Clear all bodies from the simulation
- **Mouse Hover**: Highlight and select celestial bodies
- **Mouse Hover and Right Click**: Delete a celestial body

Use the UI components to add new celestial bodies, adjust their properties, and control the simulation parameters.

## Note
- The size of each unit square in the simulation grid is a 10,000km by 10,000km square.

## Project Structure

- `src/`: Source code files
  - `main.cpp`: Application entry point
  - `app.cpp`: Main application logic
  - `manager/`: Input management
  - `physics/`: Physics simulation and celestial body logic
  - `visuals/`: Rendering and UI components
- `include/`: Header files for external libraries (Raylib, Eigen)
- `constants/`: Project constants and configuration
- `lib/`: Pre-built libraries (Raylib for Windows)
- `CMakeLists.txt`: Build configuration
- `OuterSpaceSimulation.exe`: Pre-built executable (Windows only)