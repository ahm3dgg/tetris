cmake_minimum_required(VERSION 3.29)
set(PROJECT_NAME tetris)
set(CMAKE_CXX_STANDARD 20)
project(${PROJECT_NAME} CXX)

set(PKG_CONFIG_EXECUTABLE  "")

find_package(raylib CONFIG REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(SRC_DIR src)

add_executable(${PROJECT_NAME} src/main.cpp src/block.cpp src/draw.cpp src/tetris.cpp)
target_link_libraries(${PROJECT_NAME} winmm raylib)

# add_custom_target(copy_assets ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${PROJECT_SOURCE_DIR}/assets
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
#     COMMENT "Copying assets into binary directory")

# add_dependencies(${PROJECT_NAME} copy_assets)

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         $<TARGET_FILE:SDL2::SDL2>
#         $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )