#pragma once

#include "lve_device.hpp"
#include "lve_pipeline.hpp"
#include"lve_window.hpp"
#include "lve_swap_chain.hpp"


#include <memory>
#include <vector>


namespace lve{


class FirstApp{
  public:
    //constexpre保证在编译时是已知的  
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();


    FirstApp(const FirstApp&) = delete;  //禁止copy这个class的object
    FirstApp &operator=(const FirstApp&) = delete;


    void run();

  private:

    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();


    LveWindow lveWindow_app{WIDTH , HEIGHT, "hello vulkan"} ;
    LveDevice lveDevice_app{lveWindow_app};
    LveSwapChain lveSwapChain_app{ lveDevice_app, lveWindow_app.getExtend()};
    
    
    std::unique_ptr<LvePipeline> lvePipeline ;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    
};


}