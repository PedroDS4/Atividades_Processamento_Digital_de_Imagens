cmake_minimum_required(VERSION 3.0.0)
project(troca_regioes VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(troca_regioes troca_regioes.cpp)
target_link_libraries(troca_regioes ${OpenCV_LIBS})

