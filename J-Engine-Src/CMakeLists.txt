# Specify minimum supported cmake version
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the project name
project(JEngine-Game)

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

add_subdirectory("J-Engine")
add_subdirectory("Game")
add_subdirectory("Game-Bootstrap")
add_subdirectory("J-Engine-Editor")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT J-Engine-Editor)
