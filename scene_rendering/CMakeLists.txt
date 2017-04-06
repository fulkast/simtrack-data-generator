cmake_minimum_required( VERSION 2.8)
project(scene_rendering)

find_package(utilities)

add_definitions("-std=c++11")

include_directories(
  include
  ${utilities_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/rendered_scene.cpp
)
