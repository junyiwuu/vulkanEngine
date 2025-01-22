#pragma once

#include"lve_window.hpp"

namespace lve{
    class FirstApp{
      public:
        //constexpre保证在编译时是已知的  
        static constexpr int width = 800;
        static constexpr int height = 600;

        void run();

      private:
        LveWindow LveWindow_app{width , height, "hello vulkan"} ;
    };
}