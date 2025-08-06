#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 position, float cameraSpeed)
    : m_position(position), m_cameraSpeed(cameraSpeed), m_zoom(1.0f), m_viewMatrix(1.0f)
{
}

void Camera::Move(glm::vec2 movementInput)
{
    m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(movementInput, 0.0f));
}
