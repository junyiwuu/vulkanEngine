#include "lve_window.hpp"
#include <stdexcept>

namespace lve{  //相当于写lve::LveWindow::Lvewindow。。只是这样每次都要写lve不方便

//constructor
LveWindow::LveWindow( int w, int h, std::string name):
    width{w} , height{h} , windowName{name} {
        initWindow();
}


//destructor
LveWindow::~LveWindow(){
    glfwDestroyWindow(window);
    glfwTerminate();
}


//private --> initWindow
void LveWindow::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API , GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow( width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizedCallback);
    //it only expect the correct structure
}


//createWindowSurface
void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
    if(glfwCreateWindowSurface(instance, window, nullptr, surface)!=VK_SUCCESS){
        throw std::runtime_error ("failed to create window surface");
    }
}

 void LveWindow::framebufferResizedCallback(GLFWwindow *window, int width, int height){
    auto lveWindow_app = reinterpret_cast<LveWindow *>(glfwGetWindowUserPointer(window));
    //converts glfw window pointer to LveWindow pointer
    lveWindow_app->framebufferResized = true;
    lveWindow_app->width = width;
    lveWindow_app->height = height;

    //update the size: width and height
 };
}