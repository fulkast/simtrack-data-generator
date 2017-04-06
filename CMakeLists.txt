cmake_minimum_required( VERSION 2.8)
project(simtrack_data_generator)

find_package(interface REQUIRED)
add_subdirectory(scene_rendering)

include_directories(scene_rendering/include)

add_executable(generateARFeatures src/generate-AR-features.cpp)

target_link_libraries(generateARFeatures scene_rendering)


### TESTS
enable_testing()
find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -ansi -Wno-deprecated -pthread)
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(foo tests/h5_param_parsing/h5_param_parsing.cpp)
target_link_libraries(foo ${GTEST_BOTH_LIBRARIES} pthread scene_rendering)
add_test(AllTestsInFoo foo)
