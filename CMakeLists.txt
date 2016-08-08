cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(diamond2osc main.cpp)

target_link_libraries(diamond2osc lo diamondapparatus -lpthread)

