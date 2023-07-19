cmake_minimum_required(VERSION 3.5)
project(MyProject)

include_directories(inc)

file(GLOB SOURCES "src/*.cpp")
add_executable(network_interfaces ${SOURCES})

if(WIN32)
    target_link_libraries(network_interfaces PRIVATE iphlpapi ws2_32)
else()
endif()