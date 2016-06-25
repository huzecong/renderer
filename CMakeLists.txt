cmake_minimum_required(VERSION 3.5)
project(renderer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp Utility.h Canvas.h Object.h Ray.h Scene.cpp Scene.h Object.cpp Material.cpp Material.h Light.h Common.h scenes/default.h scenes/cornell.h)
add_executable(renderer ${SOURCE_FILES})
target_link_libraries(renderer ${OpenCV_LIBS})