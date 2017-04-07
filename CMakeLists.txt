cmake_minimum_required( VERSION 2.8)
project(simtrack_data_generator)

find_package(interface REQUIRED)
find_package(OpenCV 3.2.0 REQUIRED)
add_subdirectory(scene_rendering)

include_directories(scene_rendering/include
        $interface_INCLUDE_DIRS})

add_executable(generateARFeatures src/generate-AR-features.cpp)

target_link_libraries(generateARFeatures
        scene_rendering
        ${OpenCV_LIBS})


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
