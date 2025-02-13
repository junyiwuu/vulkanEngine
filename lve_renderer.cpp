

#include "lve_renderer.hpp"
#include <stdexcept>
#include <array>
#include <cassert>


namespace lve{



//constructor
LveRenderer::LveRenderer(LveWindow &window , LveDevice &device) : 
        lveWindow_app{window} , lveDevice_app{device}  {

    recreateSwapChain();
    createCommandBuffers();
}

LveRenderer::~LveRenderer(){ freeCommandBuffers(); }


void LveRenderer::recreateSwapChain(){
    auto extent = lveWindow_app.getExtend();
    while (extent.width == 0 || extent.height == 0){
        extent = lveWindow_app.getExtend();  //等待恢复窗口大小，更新最新的窗口大小

        glfwWaitEvents(); //put the calling thread to sleep until at least one event is available in the event queue
        //程序暂停
    }
    vkDeviceWaitIdle(lveDevice_app.device());

    if (lveSwapChain_app == nullptr){
    lveSwapChain_app = std::make_unique<LveSwapChain>(lveDevice_app, extent);}
    else{
        std::shared_ptr<LveSwapChain> oldSwapChain = std::move(lveSwapChain_app);
        lveSwapChain_app = std::make_unique<LveSwapChain>(lveDevice_app, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*lveSwapChain_app.get())){
            throw std::runtime_error("swap chain image (or depth) format has changed! ");
        }
    }
}




void LveRenderer::createCommandBuffers(){
    commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = lveDevice_app.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers (
            lveDevice_app.device(),
            &allocInfo,
            commandBuffers.data() )    != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to allocate command buffers!");
            }


}

void LveRenderer::freeCommandBuffers(){
    vkFreeCommandBuffers(
        lveDevice_app.device(), lveDevice_app.getCommandPool(),
        static_cast<float>(commandBuffers.size()), 
        commandBuffers.data()     );
    commandBuffers.clear();
}


VkCommandBuffer LveRenderer::beginFrame(){
    assert(!isFrameStarted && "cant call beginFrame while already in progress");

    auto result = lveSwapChain_app->acquireNextImage(&currentImageIndex);

    //check issues
    if (result == VK_ERROR_OUT_OF_DATE_KHR){
        recreateSwapChain();
        return nullptr; }
    if(result!=VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    isFrameStarted = true;
    auto commandBuffer = getCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    return commandBuffer;

};
void LveRenderer::endFrame(){
    assert(isFrameStarted && "cant call endFrame while frame is not in progress");
    auto commandBuffer = getCurrentCommandBuffer();

    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
        throw std::runtime_error("failed to record command buffer!"); }

    auto result = lveSwapChain_app->submitCommandBuffers(&commandBuffer, &currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result== VK_SUBOPTIMAL_KHR || lveWindow_app.wasWindowResized()){
        lveWindow_app.resetWindowResizedFlag();
        recreateSwapChain(); }
    else if(result != VK_SUCCESS){
        throw std::runtime_error("Failed to present swap chain image!"); 
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
};


void LveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
    assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");
    
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = lveSwapChain_app->getRenderPass();
    renderPassInfo.framebuffer = lveSwapChain_app->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0 , 0 };
    renderPassInfo.renderArea.extent = lveSwapChain_app->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(lveSwapChain_app->getSwapChainExtent().width);
    viewport.height = static_cast<float>(lveSwapChain_app->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor { {0,0}, lveSwapChain_app->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

};


void LveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
    assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");
    
    vkCmdEndRenderPass(commandBuffer);
};


} // namespace lve
