cmake_minimum_required( VERSION 2.8)
project(simtrack_data_generator)

find_package(interface REQUIRED)
add_subdirectory(scene_rendering)

include_directories(scene_rendering/include)

add_executable(generateARFeatures src/generate-AR-features.cpp)
