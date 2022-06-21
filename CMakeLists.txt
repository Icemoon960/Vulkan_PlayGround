cmake_minimum_required(VERSION 3.15)
project(Vulkan_PlayGround CXX)

add_executable(Vulkan_PlayGround src/HelloWorldTriangleApplication.cpp src/main.cpp)

target_compile_options(Vulkan_PlayGround PUBLIC "-std=c++20")
target_link_libraries(Vulkan_PlayGround PUBLIC "-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi")

target_include_directories(Vulkan_PlayGround PUBLIC include)

install(TARGETS Vulkan_PlayGround)

