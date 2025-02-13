#pragma once
#include "lve_model.hpp"
#include <memory>

//libs
#include <glm/gtc/matrix_transform.hpp>

namespace  lve{



struct TransformComponent{
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation;

    glm::mat4 mat4() {
      auto transform = glm::translate(glm::mat4{1.f}, translation);  //create translation matrix

      //use yxz rotation
      transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
      transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
      transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});
      transform = glm::scale(transform, scale);
      return transform;   
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
    TransformComponent transform{};

  private:
    LveGameObject(id_t objId) : id{objId} {}
    id_t id;


};

} // namespace  lve{

