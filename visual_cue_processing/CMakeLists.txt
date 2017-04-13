cmake_minimum_required( VERSION 2.8 )
project(visual_cue_processing)

include_directories(/usr/local/include)

find_package(PCL 1.7 REQUIRED)

#message(SEND_ERROR ${PCL_INCLUDE_DIRS})

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable( kdtree_search src/kdtree_search.cpp)
target_link_libraries(kdtree_search ${PCL_LIBRARIES})

add_executable (matrix_transform src/matrix_transform.cpp)
target_link_libraries (matrix_transform ${PCL_LIBRARIES})

add_executable (range_image src/range_image.cpp)
target_link_libraries (range_image ${PCL_LIBRARIES})

add_executable (pointcloud_viz src/pointcloud_viz.cpp)
target_link_libraries (pointcloud_viz ${PCL_LIBRARIES})