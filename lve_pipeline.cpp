#include "lve_pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve{

    //constructor
    lvePipeline::lvePipeline(const std::string& vertFilepath, const std::string& fragFilepath){
        createGraphicPipeline(vertFilepath, fragFilepath);
    }

    //readFile
    std::vector<char> lvePipeline::readFile(const std::string& filepath){
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

    void lvePipeline::createGraphicPipeline
        (const std::string& vertFilepath, const std::string& fragFilepath){

        //use what defined above
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout<< "Vertex Shader Code Size: "<< vertCode.size() << std::endl;
        std::cout<< "Fragment Shader Code Size: "<< fragCode.size() << std::endl;
        

    }
}