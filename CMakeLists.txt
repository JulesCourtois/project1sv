cmake_minimum_required (VERSION 2.6)
project (neuron)

enable_testing()
add_subdirectory(googletest)
include_directories(${googletest_SOURCE_DIR}/include ${googletest_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-W -Wall -pedantic -std=c++11")
add_executable (neuron Neuron.cpp main.cpp)

add_executable (neuron_unittest Neuron.cpp neuron_unittest.cpp)
target_link_libraries(neuron_unittest gtest gtest_main)

add_test(neuron_unittest neuron_unittest)
