cmake_minimum_required(VERSION 2.8)
project(renderer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -DNDEBUG")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp Utility.h Canvas.h Object.h Ray.h Scene.cpp Scene.h Material.cpp Material.h Light.h Common.h scenes/cornell.h scenes/depth_of_field.h scenes/environment_lighting.h scenes/metallic.h)
add_executable(renderer ${SOURCE_FILES})
target_link_libraries(renderer ${OpenCV_LIBS})