#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "simple_render_system.hpp"
#include <stdexcept>
#include <array>


namespace lve{


struct SimplePushConstantData{
    glm::mat4 transform{1.0f};
    alignas(16) glm::vec3 color;    //color按照16ytes对齐
};

//constructor
SimpleRenderSystem::SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass) : lveDevice{device} {
    createPipelineLayout();
    createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem(){
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

 


void SimpleRenderSystem::createPipelineLayout(){


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
            lveDevice.device(),
            &pipelineLayoutInfo,
            nullptr,
            &pipelineLayout)  != VK_SUCCESS) 
            
            {
                throw std::runtime_error("failed to create pipeline layout!");
            }
}



//now will be call when recreateSwapChain
void SimpleRenderSystem::createPipeline(VkRenderPass renderPass){
    assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo (pipelineConfig);
    
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;

    lvePipeline = std::make_unique<LvePipeline>(
        lveDevice,
        "../shaders/simple_shader.vert.spv",
        "../shaders/simple_shader.frag.spv",
        pipelineConfig );

}
 

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer , std::vector<LveGameObject> &gameObjects){
    
    lvePipeline->bind(commandBuffer);

    for(auto& obj : gameObjects){
        obj.transform.rotation.y = glm::mod(obj.transform.rotation.y +  0.01f,   glm::two_pi<float>());
        
        SimplePushConstantData push{};
        push.color = obj.color;
        push.transform = obj.transform.mat4();

        //record it into command buffer
        vkCmdPushConstants(
            commandBuffer, pipelineLayout, 
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0, sizeof(SimplePushConstantData), &push );

        obj.model -> bind(commandBuffer);
        obj.model -> draw(commandBuffer);
        

    }

}

 

} // namespace lve
