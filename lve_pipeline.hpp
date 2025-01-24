#pragma once

#include "lve_device.hpp"

#include <string>
#include <vector>

namespace lve{

//specifying how we want to configure our pipeline
//application layer code easily able to configure our pipeline completely, 
//also share that configureation cross different pipeline


//---------------------------------------

    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;

        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;

        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;

        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;

        
    };       

//---------------------------------------------------
    class LvePipeline{

      public:
        LvePipeline(
            LveDevice& device,
            const std::string& vertFilepath, 
            const std::string& fragFilepath,
            const PipelineConfigInfo& configInfo
            );
        ~LvePipeline();

        //protection
        LvePipeline(const LvePipeline& ) = delete;
        void operator =(const LvePipeline& ) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,  uint32_t height);




      private:
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicPipeline(
            const std::string& vertFilepath, 
            const std::string& fragFilepath,
            const PipelineConfigInfo& configInfo
            );


        //pointer to a pointer
        void createShaderModule( const std::vector<char>& code, VkShaderModule* shaderModule);
        
        //only when we have implicit relationship that our member variable, in this case, the device
        //will outlive any instances of the containing class that depend on it
        //pipeline fundamentally needs a device to exist
        //aggregation here

        LveDevice& lveDevice;
        VkPipeline graphicPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;

    };
} //namespace lve