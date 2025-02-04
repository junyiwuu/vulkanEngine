#pragma once
#include "lve_model.hpp"
#include <memory>

namespace  lve{



struct Transform2dComponent{
    glm::vec2 translation{};
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2_transform() {
        const float s = glm::sin(rotation);
        const float c = glm::cos(rotation);
        glm::mat2 rotMatrix{{c,s} , {-s,c}};

        glm::mat2 scaleMat{  {scale.x ,0} , {0, scale.y}};  //columns
        //return rotMatrix * scaleMat; 
        return  scaleMat*rotMatrix ;
        
        }
};

class LveGameObject {


  public:
    using id_t = unsigned int;
    //static 无需实例即可调用，在哪都能调用
    static LveGameObject createGameObject() {
        static id_t currentId = 0; //只初始化一次 
        return LveGameObject{currentId++};
    }

    LveGameObject(const LveGameObject&) = delete;
    LveGameObject &operator = (const LveGameObject&) = delete;

    LveGameObject(LveGameObject&&) = default;
    LveGameObject  &operator=(LveGameObject &&) = default;

    id_t getId() {return id;}

    std::shared_ptr<LveModel> model{};
    glm::vec3 color{};
    Transform2dComponent transform2d{};

  private:
    LveGameObject(id_t objId) : id{objId} {}
    id_t id;


};

} // namespace  lve{

