@echo off
set BUILD_DIR=build
set CMAKE_GENERATOR=Ninja

:: Build Debug
if "%1%"=="build_debug" (
    mkdir %BUILD_DIR%
    cd %BUILD_DIR%
    cmake -DCMAKE_BUILD_TYPE=Debug -G %CMAKE_GENERATOR% ..
    cmake --build .
    cd ..
)

:: Build Release
if "%1%"=="build_release" (
    mkdir %BUILD_DIR%
    cd %BUILD_DIR%
    cmake -DCMAKE_BUILD_TYPE=Release -G %CMAKE_GENERATOR% ..
    cmake --build .
    cd ..
)

:: Run Debug
if "%1%"=="run_debug" (
    "./bin/Debug/tetris.exe"
)

:: Run Release
if "%1%"=="run_release" (
    "./bin/Release/tetris.exe"
)

:: Help
if "%1%"=="" (	
    echo Usage: build.bat [build_debug | build_release | run_debug | run_release]
)