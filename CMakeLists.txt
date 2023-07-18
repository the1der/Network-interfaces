cmake_minimum_required(VERSION 3.5)
project(MyProject)

# Add source files
file(GLOB SOURCES "src/*.cpp")

include_directories(inc)

# Add executable
add_executable(network_interfaces ${SOURCES})

