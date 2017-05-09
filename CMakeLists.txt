cmake_minimum_required( VERSION 2.8)

include_directories(/usr/local/include/pcl-1.8)

project(simtrack_data_generator)

find_package(OpenCV 3.2.0 REQUIRED)
find_package(interface REQUIRED)
find_package(utilities REQUIRED)
find_package(rendering REQUIRED)
find_package(CUDA REQUIRED)
find_package(pose_estimation REQUIRED)
add_definitions("-std=c++11")


include_directories(include
        ${interface_INCLUDE_DIRS}
        )

add_library(${PROJECT_NAME}
        src/rendered_scene.cpp src/track_single_object.cpp
        )

target_link_libraries(${PROJECT_NAME}
        ${interface_LIBRARIES})

add_executable(render_example src/render_example.cpp)

target_link_libraries(render_example
        ${OpenCV_LIBS}
        ${interface_LIBRARIES}
        ${PROJECT_NAME}
        ${utilities_LIBRARIES}
        ${rendering_LIBRARIES}
        ${CUDA_LIBRARIES}
        ${pose_estimation_LIBRARIES}

        )

add_executable(tracker_unit_test src/tracker_unit_test.cpp)

target_link_libraries(tracker_unit_test
        ${OpenCV_LIBS}
        ${interface_LIBRARIES}
        ${PROJECT_NAME}
        ${utilities_LIBRARIES}
        ${rendering_LIBRARIES}
        ${CUDA_LIBRARIES}
        ${pose_estimation_LIBRARIES}
        )


add_executable(write_query_points src/write_query_points.cpp)

target_link_libraries(write_query_points
        ${OpenCV_LIBS}
        ${interface_LIBRARIES}
        ${PROJECT_NAME}
        ${utilities_LIBRARIES}
        ${rendering_LIBRARIES}
        ${CUDA_LIBRARIES}
        ${pose_estimation_LIBRARIES}
        )

### TESTS
enable_testing()
find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -ansi -Wno-deprecated -pthread -std=c++11)
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS}
        ${interface_INCLUDE_DIRS})

add_executable(parse_h5_param_test tests/h5_param_parsing/h5_param_parsing.cpp)
target_link_libraries(parse_h5_param_test
        ${GTEST_BOTH_LIBRARIES}
        pthread
        scene_rendering
        ${OpenCV_LIBS}
)
add_test(AllParamParseTests parse_h5_param_test)
