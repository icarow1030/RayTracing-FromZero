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