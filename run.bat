@echo off
set EXE_PATH="C:\Users\icaro\CLionProjects\RayTracing1\build\Debug\RayTracing1.exe"

echo [1/3] Configurando o CMake...
cmake -B build

echo [2/3] Compilando o projeto...
cmake --build build

if %ERROR LEVEL% EQU 0 (
    echo [3/3] Executando o motor...
    echo.
    %EXE_PATH%
) else (
    echo.
    echo Erro: A compilacao falhou. O executavel nao sera iniciado.
)

pause