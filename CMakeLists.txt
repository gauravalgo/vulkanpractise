cmake_minimum_required(VERSION 3.0)

project(vulkan_test)

add_executable(vulkan_test main.cpp)

install(TARGETS vulkan_test RUNTIME DESTINATION bin)
