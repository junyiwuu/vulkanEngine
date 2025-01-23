#include "first_app.hpp"

namespace lve{
    void FirstApp::run(){
        while (!lveWindow_app.shouldClose()){
            //while our window doesnt want to close, pull window events
            glfwPollEvents();
        }
    }
} // namespace lve
