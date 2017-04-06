cmake_minimum_required( VERSION 2.8)
project(scene_rendering)

find_package(utilities REQUIRED)
#find_package(Boost REQUIRED COMPONENTS system)

add_definitions("-std=c++11")

include_directories(
  include
  ${utilities_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/rendered_scene.cpp
)

#message(SEND_ERROR ${utilities_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
  ${utilities_LIBRARIES}
#  ${Boost_LIBRARIES}
)
