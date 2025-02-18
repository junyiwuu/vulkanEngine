#pragma once

#include "lve_device.hpp"
#include"lve_window.hpp"
#include "lve_swap_chain.hpp"

#include <cassert>
#include <memory>
#include <vector>


namespace lve{


class LveRenderer{
  public:
    LveRenderer(LveWindow &window, LveDevice &device);
    ~LveRenderer();

    LveRenderer(const LveRenderer&) = delete;  //禁止copy这个class的object
    LveRenderer &operator=(const LveRenderer&) = delete;

    VkRenderPass getSwapChainRenderPass() const {return lveSwapChain_app->getRenderPass(); }
    float getAspectRatio() const {return lveSwapChain_app->extentAspectRatio(); }
    bool isFrameInProgress() const {return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const {
      assert(isFrameStarted && "cannot get frame index when frame not in progress");
      return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

  private:

    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();


    LveWindow& lveWindow_app ;
    LveDevice& lveDevice_app;
    std::unique_ptr<LveSwapChain> lveSwapChain_app;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex = 0;
    bool isFrameStarted;  //not yet
};


}