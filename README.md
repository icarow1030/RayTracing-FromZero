# C++ Path Tracer

A physically-based ray tracer built from scratch in standard C++. 

This project is a CPU-based rendering engine that simulates the physical behavior of light, including accurate reflections, refractions, and depth of field. It was developed without the use of external graphical APIs (like OpenGL or DirectX), relying entirely on custom vector mathematics and ray-geometry intersection algorithms.

![Render Preview](https://github.com/icarow1030/RayTracing-FromZero/blob/main/images/output.png)
*Rendered at 1080p with 100 samples per pixel and a recursion depth of 50.*

## Features

*   **Core Ray Tracing:** Implementation of ray-sphere intersections, surface normal calculations, and recursive light scattering.
*   **Physically Based Materials:**
    *   **Lambertian (Diffuse):** Simulates matte surfaces with accurate color bleeding and random light scattering.
    *   **Metal:** Simulates reflective surfaces with a controllable "fuzziness" parameter for polished or brushed metal looks.
    *   **Dielectric (Glass):** Simulates transparent materials using Snell's Law for refraction, handling Total Internal Reflection, and employing the Schlick approximation for view-dependent reflectivity.
*   **Virtual Camera System:** 
    *   Adjustable Field of View (FOV).
    *   Simulated physical lens with Defocus Blur (Depth of Field).
*   **Anti-Aliasing:** Multisampling per pixel for smooth edges and noise reduction.

## Architecture & Performance Bottleneck

Currently, the engine runs sequentially on the CPU. While the recursive nature of the `ray_color` function is elegant in standard C++, it presents a significant performance bottleneck. Calculating millions of rays with multiple bounces limits the engine's ability to render high-resolution images quickly. 

### Roadmap: GPU Acceleration (CUDA)
The immediate next step for this project is a complete architectural refactor to port the computational load to the GPU using **NVIDIA CUDA**. This will involve:
*   Flattening the Object-Oriented structure (removing virtual functions and v-tables) to avoid branch divergence.
*   Replacing recursive ray tracing with an iterative approach to preserve GPU stack memory.
*   Implementing parallel random number generation (cuRAND).

## Usage & Scene Configuration

The `main.cpp` file acts as your virtual photographic studio. Inside the `main()` function, you can adjust the camera settings for performance/quality and build your own custom 3D scenes.

### 1. Adjusting Render Quality
Because this engine runs sequentially on the CPU, high-resolution renders with many samples will take time. You should adjust the camera parameters according to your current needs (quick previews vs. high-quality outputs):
```cpp
camera cam;

cam.aspect_ratio      = 16.0 / 9.0;
cam.image_width       = 400; // E.g., 400 for quick previews, 1920 for full HD renders
cam.samples_per_pixel = 10;  // E.g., 10 for fast tests, 100-500+ for noise-free images
cam.max_depth         = 10;  // E.g., 10-50 for realistic light bounces
```

*   `image_width`: The horizontal resolution of the generated image.
*   `samples_per_pixel`: The number of random rays shot for each pixel. Higher values reduce noise (aliasing) but increase render time linearly.
*   `max_depth`: The maximum number of times a light ray can bounce off objects before being absorbed.

### 2. Building Your Own Scene
You can populate the `world` object with spheres and different physically-based materials. The engine supports diffuse, metallic, and glass surfaces. Here is the basic syntax to create materials and add objects to the environment:
```cpp
hittable_list world;

// 1. Create your materials
auto mat_ground = std::make_shared<lambertian>(color(0.5, 0.5, 0.5)); // Matte/Diffuse
auto mat_glass  = std::make_shared<dielectric>(1.5);                  // Refractive (Index of Refraction)
auto mat_metal  = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0); // Reflective (color, fuzziness)

// 2. Add spheres to the world: std::make_shared<sphere>(center_point, radius, material)
// Ground (a giant sphere)
world.add(std::make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, mat_ground));

// Scene Objects
world.add(std::make_shared<sphere>(point3(-1.0, 1.0, -2.0), 1.0, mat_glass));
world.add(std::make_shared<sphere>(point3( 1.0, 1.0, -2.0), 1.0, mat_metal));
```

## How to Build and Run

This project uses **CMake** for its build system. 

### Windows
For Windows users, a batch script is provided to automate the build and execution process. Simply double-click the `run.bat` file in the root directory, or run it via terminal:
```cmd
.\run.bat
```

*Note: The `run.bat` script executes the following steps:*
```bat
@echo off
set EXE_PATH="build\Debug\RayTracing1.exe"

echo [1/3] Configuring CMake...
cmake -B build

echo [2/3] Building the project...
cmake --build build

if %ERRORLEVEL% EQU 0 (
    echo [3/3] Running the engine...
    echo.
    %EXE_PATH%
) else (
    echo.
    echo Error: Build failed. The executable will not start.
)

pause
```

### Manual Build (Cross-Platform)
If you are on Linux, macOS, or prefer to build manually via terminal, ensure you have CMake installed and run the following commands from the root directory:
```bash
# 1. Configure the project
cmake -B build

# 2. Build the executable
cmake --build build

# 3. Run the engine (the path might vary slightly depending on your OS/Compiler)
./build/Debug/RayTracing1
```
