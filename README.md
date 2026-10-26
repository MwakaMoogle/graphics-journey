# graphics-journey
A cross-platform OpenGL (C++) workspace documenting my progression in graphics programming.

This repository tracks my journey into low-level graphics programming using modern OpenGL (Core Profile) and C++. 

My ultimate goal for this workspace is to build a physics-based space simulation featuring orbital mechanics, custom shaders, and complex lighting models. Currently, I am working through the foundational graphics pipeline concepts.

## Tech Stack
* **Language:** C++17
* **Graphics API:** OpenGL 3.3+ (Core Profile)
* **Windowing/Input:** [GLFW](https://www.glfw.org/) (Auto-fetched via CMake)
* **Extension Loader:** [GLAD](https://glad.dav1d.de/)
* **Build System:** CMake

## Cross-Platform Build System
This project is configured to compile seamlessly on both **macOS** (using the native OpenGL framework) and **Linux** (using standard Mesa drivers). Dependencies like GLFW are automatically downloaded and compiled from source using CMake's `FetchContent`, keeping the repository completely self-contained.

### Prerequisites
**macOS:**
* `cmake` (via Homebrew)
* Apple's Command Line Tools

**Fedora Linux:**
* X11/Wayland development headers:
  ``` bash
  sudo dnf install gcc-c++ cmake make libX11-devel libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel mesa-libGL-devel
  ```

### Building the Project

Clone the repository and build using standard CMake commands:
``` bash
git clone [https://github.com/MwakaMoogle/graphics-journey.git](https://github.com/MwakaMoogle/graphics-journey.git)
cd graphics-journey
mkdir build
cd build
cmake ..
make
```
Executable for each tutorial/module will be generated directly in the build/ directory.

## Project Structure
``` plaintext
.
├── CMakeLists.txt      # Cross-platform build configuration
├── assets/             # Shaders, textures, and 3D models (auto-copied to build/)
├── include/            # Shared headers (GLAD, stb_image, etc.)
└── src/                # C++ source code for individual rendering programs
```

