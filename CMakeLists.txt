#Minumum cmake version
cmake_minimum_required(VERSION 3.10)

#project name and version
project(vulkanTest VERSION 1.0 LANGUAGES CXX)

#specify the c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#inlcude directories for GLFW and vulkan
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)



# file(GLOB SOURCES "*.cpp")
#add the executable target
add_executable(vulkanTest 
        main.cpp 
        first_app.cpp 
        lve_window.cpp 
        lve_device.cpp
        lve_pipeline.cpp
        )


#link the required libraries
#第一个参数是链接库的目标
target_link_libraries(vulkanTest Vulkan::Vulkan glfw)