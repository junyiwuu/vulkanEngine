#pragma once

#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_window.hpp"
#include "lve_renderer.hpp"


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
    void loadGameObjects();


    LveWindow lveWindow_app{WIDTH , HEIGHT, "hello vulkan"} ;
    LveDevice lveDevice_app{lveWindow_app};
    LveRenderer lveRenderer{lveWindow_app, lveDevice_app};
 
    std::vector<LveGameObject> gameObjects;
    
};


}