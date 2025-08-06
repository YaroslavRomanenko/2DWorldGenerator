#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 position)
    : m_position(position), m_viewMatrix(1.0f), m_cameraSpeed(250.0f), m_zoom(1.0f), m_accelerate(false)
{
    UpdateViewMatrix();
}

void Camera::Move(glm::vec2 displacement)
{
    m_position += displacement;
    UpdateViewMatrix();
}

void Camera::AdjustZoom(float yOffset, const glm::vec2& mousePos, const glm::vec2& windowSize)
{
    glm::vec2 mouseOffsetFromCenter = mousePos - (windowSize / 2.0f);
    glm::vec2 worldOffsetFromCamera = mouseOffsetFromCenter / m_zoom;
    glm::vec2 worldPosUnderMouse = m_position + worldOffsetFromCamera;    

    float oldZoom = m_zoom;
    float zoomSensivity = 0.05f;
    m_zoom += yOffset * zoomSensivity;
    m_zoom = glm::clamp(m_zoom, 0.25f, 2.0f);

    m_position = worldPosUnderMouse + (m_position - worldPosUnderMouse) * (oldZoom / m_zoom);

    UpdateViewMatrix();
}

void Camera::Reset() 
{
    m_position = glm::vec2(0.0f);
    m_zoom = 1.0f;
    UpdateViewMatrix();  
}

void Camera::UpdateViewMatrix()
{
    glm::vec2 translation(-m_position.x, -m_position.y);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(translation, 0.0f));
    transform = glm::scale(transform, glm::vec3(m_zoom, m_zoom, 1.0f));
    
    m_viewMatrix = transform;
}
