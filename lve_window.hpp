#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>


namespace lve {


    class LveWindow{
      private:

        void initWindow();
        const int width;
        const int height;
        std::string windowName;

        //GLFWwindow是一个数据类型， *指针. 指向window的内存位置
        GLFWwindow *window;

      public:
        //如果constructor是private那么就无法在为外部创建object
        LveWindow(int w, int t, std::string name);
        ~LveWindow();

        //创建两个windows，互相指向同一个glfwwindow，如果两个都destroy那么就destroy两次，会crash
        //所有的windows应该都是独一无二的
        LveWindow(const LveWindow&) = delete;  //禁止copy这个class的object
        LveWindow &operator=(const LveWindow&) = delete;

        bool shouldClose() {return glfwWindowShouldClose(window);}
        //VkExtent2D getExtend() {return {width, height}; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    };
}