#include "lve_pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve{

    //constructor
    LvePipeline::LvePipeline (
        LveDevice& device,
        const std::string& vertFilepath, 
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo
        ) :lveDevice {device} 
        {
            createGraphicPipeline(vertFilepath, fragFilepath, configInfo);
        }


        

    //readFile
    std::vector<char> LvePipeline::readFile(const std::string& filepath){
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};
        
        //error check
        if (!file.is_open()){
            throw std::runtime_error("fail to open file: " + filepath);
            //filepath is incorrect or no permission to open the file
        };

        size_t fileSize = static_cast<size_t>(file.tellg());
        //static_cast: 编译时完成类型转换，只允许安全的转换

        //define one buffer
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
        //一个文件一个buffer. stored in the memory, heap
    }



    void LvePipeline::createGraphicPipeline
        (const std::string& vertFilepath, 
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo
        ){

        //use what defined above
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout<< "Vertex Shader Code Size: "<< vertCode.size() << std::endl;
        std::cout<< "Fragment Shader Code Size: "<< fragCode.size() << std::endl;
    }

    
    void LvePipeline::createShaderModule( const std::vector<char>& code, VkShaderModule* shaderModule){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule( lveDevice.device() , &createInfo, nullptr, shaderModule) != VK_SUCCESS){
            throw std::runtime_error("failed to create shader module");
        }

    }

    PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width,  uint32_t height){
            PipelineConfigInfo configInfo{};



            return configInfo;
    }

        
}