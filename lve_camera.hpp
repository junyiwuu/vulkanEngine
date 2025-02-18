#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve{

class LveCamera {
    public:

    void setOrthoProjection(float left, float right, float top, float bottom, float near, float far);
    void setPerspProjection(float fovy, float aspect, float near, float far);

    void setCamDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setCamTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setCamYXZ(glm::vec3 position, glm::vec3 rotation);

    const glm::mat4& getProjection() const {return projectionMatrix; }
    const glm::mat4& getView() const {return viewMatrix; }
   
    private:
    glm::mat4 projectionMatrix{1.f};  //create identity matrix
    glm::mat4 viewMatrix{1.f};
};


}