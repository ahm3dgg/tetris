cmake_minimum_required(VERSION 3.29)

set(PROJECT_NAME tetris)
set(CMAKE_CXX_STANDARD 20)
project(${PROJECT_NAME} CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release)

add_executable(${PROJECT_NAME} WIN32 src/main.cpp)

set(RAYLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib)

include_directories(${RAYLIB_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_DIR}/lib)
target_link_libraries(${PROJECT_NAME}  user32 raylib winmm)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets
    $<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/assets,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/assets>
)