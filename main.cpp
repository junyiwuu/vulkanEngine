#include "first_app.hpp"

#include <cstdlib>
#include <iostream>

#include <stdexcept>

int main(){
    lve::FirstApp app{};

    try{
        app.run();
    }catch (const std::exception &e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // 打印所有可用的layers
    for (const auto& layer : availableLayers) {
        std::cout << "Available layer: " << layer.layerName << std::endl;
    }



    return EXIT_FAILURE;
}