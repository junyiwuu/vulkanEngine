#pragma once

#include"lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"

namespace lve{
    class FirstApp{
      public:
        //constexpre保证在编译时是已知的  
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();

      private:
        LveWindow lveWindow_app{WIDTH , HEIGHT, "hello vulkan"} ;
        LveDevice lveDevice_app{lveWindow_app};
        LvePipeline lvePipeline_app{
            lveDevice_app, 
            "../shaders/simple_shader.vert.spv", 
            "../shaders/simple_shader.frag.spv",
            LvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}