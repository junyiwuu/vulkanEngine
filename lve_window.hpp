#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>


namespace lve {


    class LveWindow{
      private:
        static void framebufferResizedCallback(GLFWwindow *window, int width, int height);
        void initWindow();
        int width;
        int height;
        bool framebufferResized = false;

        std::string windowName;
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
        VkExtent2D getExtend() {return { 
            static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

        bool wasWindowResized() {return framebufferResized; }
        void resetWindowResizedFlag() {framebufferResized = false; }
        GLFWwindow *getGLFWwindow() const {return window; }



        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    };
}