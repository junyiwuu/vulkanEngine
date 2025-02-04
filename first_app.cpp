#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "first_app.hpp"
#include <stdexcept>
#include <array>


namespace lve{


struct SimplePushConstantData{
    glm::mat2 transform{1.0f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;    //color按照16ytes对齐
};

//constructor
FirstApp::FirstApp(){
    loadGameObjects();
    createPipelineLayout();
    recreateSwapChain();
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


void FirstApp::loadGameObjects(){
    std::vector<LveModel::Vertex> vertices{  
        {{0.0f, -0.5f} ,  {1.0f, 0.0f, 0.0f}} , 
        {{0.5f, 0.5f} , {0.0f, 1.0f, 0.0f}} , 
        {{-0.5f, 0.2f} , {0.0f, 0.0f, 1.0f}}
        };
    auto lveModel = std::make_shared<LveModel>(lveDevice_app, vertices);
    auto triangle = LveGameObject::createGameObject();
    triangle.model = lveModel;
    triangle.color = {.1f, .8f, .1f};
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale = {2.f, 0.3f};
    triangle.transform2d.rotation = 0.25 * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));


}



void FirstApp::createPipelineLayout(){


    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;  //数据起始位置为0
    pushConstantRange.size = sizeof(SimplePushConstantData);  //pushconstant的大小

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{} ;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount =1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    
    if (vkCreatePipelineLayout(
            lveDevice_app.device(),
            &pipelineLayoutInfo,
            nullptr,
            &pipelineLayout)  != VK_SUCCESS) 
            
            {
                throw std::runtime_error("failed to create pipeline layout!");
            }
}



void FirstApp::recreateSwapChain(){
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
        lveSwapChain_app = std::make_unique<LveSwapChain>(lveDevice_app, extent, std::move(lveSwapChain_app));
        if (lveSwapChain_app->imageCount() != commandBuffers.size()){
            freeCommandBuffers();
            createCommandBuffers();
        }
    }
    createPipeline();
}


//now will be call when recreateSwapChain
void FirstApp::createPipeline(){
    assert(lveSwapChain_app != nullptr && "cannot create pipeline before swap chain");
    assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo (pipelineConfig);
    
    pipelineConfig.renderPass = lveSwapChain_app->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;

    lvePipeline = std::make_unique<LvePipeline>(
        lveDevice_app,
        "../shaders/simple_shader.vert.spv",
        "../shaders/simple_shader.frag.spv",
        pipelineConfig );

}



void FirstApp::createCommandBuffers(){
    commandBuffers.resize(lveSwapChain_app->imageCount());

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

void FirstApp::freeCommandBuffers(){
    vkFreeCommandBuffers(
        lveDevice_app.device(), lveDevice_app.getCommandPool(),
        static_cast<float>(commandBuffers.size()), 
        commandBuffers.data()     );
    commandBuffers.clear();
}

//https://miro.com/app/board/uXjVLov42gs=/?moveToWidget=3458764615761310086&cot=14
void FirstApp::recordCommandBuffer(int imageIndex){

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = lveSwapChain_app->getRenderPass();
    renderPassInfo.framebuffer = lveSwapChain_app->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0 , 0 };
    renderPassInfo.renderArea.extent = lveSwapChain_app->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(lveSwapChain_app->getSwapChainExtent().width);
    viewport.height = static_cast<float>(lveSwapChain_app->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor { {0,0}, lveSwapChain_app->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    renderGameObjects(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS){
        throw std::runtime_error("failed to record command buffer!");
    }
}


void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer){
    
    lvePipeline->bind(commandBuffer);

    for(auto& obj : gameObjects){
        obj.transform2d.rotation = glm::mod(obj.transform2d.rotation+0.01f, 
                    glm::two_pi<float>());
        SimplePushConstantData push{};
        push.offset = obj.transform2d.translation;
        push.color = obj.color;
        push.transform = obj.transform2d.mat2_transform();

        //record it into command buffer
        vkCmdPushConstants(
            commandBuffer, pipelineLayout, 
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0, sizeof(SimplePushConstantData), &push );

        obj.model -> bind(commandBuffer);
        obj.model -> draw(commandBuffer);
        

    }

}


void FirstApp::drawFrame(){
    uint32_t imageIndex;
    auto result = lveSwapChain_app->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR){
        recreateSwapChain();
        return;
    }

    if(result!=VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    recordCommandBuffer(imageIndex);
    result = lveSwapChain_app->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result== VK_SUBOPTIMAL_KHR || lveWindow_app.wasWindowResized()){
        lveWindow_app.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }else if(result != VK_SUCCESS){
        throw std::runtime_error("Failed to present swap chain image!"); 
    }
}

} // namespace lve
