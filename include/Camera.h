#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec2 position = glm::vec2(0.0f), float cameraSpeed = 2.5f);
    void Move(glm::vec2 movementInput);

private:
    glm::mat4 m_viewMatrix;

    glm::vec2 m_position;
    float m_cameraSpeed;
    float m_zoom;
};