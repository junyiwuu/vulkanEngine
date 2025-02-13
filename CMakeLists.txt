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

find_package(glm REQUIRED)

# file(GLOB SOURCES "*.cpp")
#add the executable target
add_executable(vulkanTest 
        main.cpp 
        first_app.cpp 
        lve_window.cpp 
        lve_device.cpp
        lve_pipeline.cpp
        lve_swap_chain.cpp
        lve_model.cpp
        lve_renderer.cpp
        simple_render_system.cpp
        )


#link the required libraries
#第一个参数是链接库的目标
target_link_libraries(vulkanTest Vulkan::Vulkan glfw)


#set the shader source and output directory
#set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
#set(SHADER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/shaders)

#file(GLOB SHADERS ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag)

add_custom_target(compile_shaders
        COMMAND chmod +x ${CMAKE_SOURCE_DIR}/compile.sh 
        COMMAND ${CMAKE_SOURCE_DIR}/compile.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "running compile.sh to compile shaders"
        VERBATIM)


add_dependencies(vulkanTest compile_shaders)
