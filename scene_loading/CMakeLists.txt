cmake_minimum_required( VERSION 2.8)
project(scene_loading)

find_package(utilities)

include_directories(
  include
  ${utilities_INCLUDE_DIRS}
)

add_definitions("-std=c++11")

add_library(${PROJECT_NAME}
  src/rendered_scene.cpp
)
