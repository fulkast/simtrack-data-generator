cmake_minimum_required( VERSION 2.8)
project(scene_rendering)

find_package(utilities REQUIRED)
find_package(rendering REQUIRED)
find_package(pose_estimation REQUIRED)
find_package(CUDA REQUIRED)
add_definitions("-std=c++11")

include_directories(
  include
  ${utilities_INCLUDE_DIRS}
  ${rendering_INCLUDE_DIRS}
  ${pose_estimation_INCLUDE_DIRS}
)

#message(SEND_ERROR   ${rendering_INCLUDE_DIRS})

add_library(${PROJECT_NAME}
  src/rendered_scene.cpp
)

target_link_libraries(${PROJECT_NAME}
  ${utilities_LIBRARIES}
  ${rendering_LIBRARIES}
  ${pose_estimation_LIBRARIES}
  ${CUDA_LIBRARIES}
)
