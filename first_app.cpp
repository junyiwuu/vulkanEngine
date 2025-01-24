#include "first_app.hpp"
#include <stdexcept>
#include <array>

namespace lve{

//constructor
FirstApp::FirstApp(){
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

FirstApp::~FirstApp(){
    vkDestroyPipelineLayout(lveDevice_app.device(), pipelineLayout, nullptr);
}


void FirstApp::run(){
    while (!lveWindow_app.shouldClose()){
        //while our window doesnt want to close, pull window events
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(lveDevice_app.device());
}





void FirstApp::createPipelineLayout(){

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{} ;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    
    if (vkCreatePipelineLayout(
            lveDevice_app.device(),
            &pipelineLayoutInfo,
            nullptr,
            &pipelineLayout)  != VK_SUCCESS) 
            
            {
                throw std::runtime_error("failed to create pipeline layout!");
            }
}



void FirstApp::createPipeline(){
    auto pipelineConfig = 
        LvePipeline::defaultPipelineConfigInfo (lveSwapChain_app.width(), lveSwapChain_app.height());
    
    pipelineConfig.renderPass = lveSwapChain_app.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;

    lvePipeline = std::make_unique<LvePipeline>(
        lveDevice_app,
        "../shaders/simple_shader.vert.spv",
        "../shaders/simple_shader.frag.spv",
        pipelineConfig );

}





void FirstApp::createCommandBuffers(){
    commandBuffers.resize(lveSwapChain_app.imageCount());

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

    for(int i=0; i<commandBuffers.size(); i++){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = lveSwapChain_app.getRenderPass();
        renderPassInfo.framebuffer = lveSwapChain_app.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0 , 0 };
        renderPassInfo.renderArea.extent = lveSwapChain_app.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {00.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        lvePipeline->bind(commandBuffers[i]);
        vkCmdDraw(commandBuffers[i] , 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);
        if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}


void FirstApp::drawFrame(){
    uint32_t imageIndex;
    auto result = lveSwapChain_app.acquireNextImage(&imageIndex);

    if(result!=VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    result = lveSwapChain_app.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

    if(result != VK_SUCCESS){
        throw std::runtime_error("Failed to present swap chain image!"); 
    }
}

} // namespace lve
