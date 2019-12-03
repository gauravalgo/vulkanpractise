cmake_minimum_required(VERSION 3.0)

project(vulkan_test)
find_package(Vulkan REQUIRED)
find_package(glfw3 3.2 REQUIRED)
add_executable(vulkan_test main.cpp)
target_link_libraries(vulkan_test vulkan glfw)
install(TARGETS vulkan_test RUNTIME DESTINATION bin)


