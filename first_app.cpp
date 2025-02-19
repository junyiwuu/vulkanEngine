#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "first_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"
#include <chrono>
#include <stdexcept>
#include <array>


namespace lve{

//constructor
FirstApp::FirstApp(){
    loadGameObjects();
}

FirstApp::~FirstApp(){ }


void FirstApp::run(){
    SimpleRenderSystem simpleRenderSystem{lveDevice_app , lveRenderer.getSwapChainRenderPass()};

    LveCamera camera{};
    //camera.setCamDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
    camera.setCamTarget(glm::vec3(-1.f, -2.f, .2f) , glm::vec3(0.f, 0.f, 2.5f));
    

    //store camera rotation and translation between frames
    //use game object to store camera position etc
    auto camObject = LveGameObject::createGameObject();
    KeyboardMovementsController cameraController{};  

    auto currentTime = std::chrono::high_resolution_clock::now();  //initialize point. initialized once only (when engine start)


    while (!lveWindow_app.shouldClose()){
        //while our window doesnt want to close, pull window events
        glfwPollEvents();
        auto newTime = std::chrono::high_resolution_clock::now(); //get new timestamp in every iteration
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;


        cameraController.moveInPlaneXZ(lveWindow_app.getGLFWwindow(), frameTime, camObject );
        camera.setCamYXZ(camObject.transform.translation , camObject.transform.rotation);

        float aspect = lveRenderer.getAspectRatio();
        //camera.setOrthoProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspProjection(glm::radians(50.f) , aspect, 0.1f, 10.f);  //here is near and far plane

        if (auto commandBuffer = lveRenderer.beginFrame()){
            lveRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
            lveRenderer.endSwapChainRenderPass(commandBuffer);
            lveRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(lveDevice_app.device());
}


// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<LveModel> createCubeModel(LveDevice& device, glm::vec3 offset) {
    LveModel::Builder modelBuilder{};
    modelBuilder.vertices = {

        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
   
        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
   
        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
   
        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
   
        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
   
        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
   
    };
    for (auto& v : modelBuilder.vertices) {
      v.position += offset;
    }
    return std::make_unique<LveModel>(device, modelBuilder);
}


void FirstApp::loadGameObjects(){
    std::shared_ptr<LveModel> lveModel = createCubeModel(lveDevice_app, {0.f, 00.f, 0.f});

    auto cube = LveGameObject::createGameObject();
    cube.model = lveModel; 
    cube.transform.translation = {0.f, 0.f, 2.5f };
    cube.transform.scale = {.5f, .5f, .5f};
    gameObjects.push_back(std::move(cube));
 
}





} // namespace lve
