#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec2 position = glm::vec2(0.0f));
    
    void Move(glm::vec2 displacement);
    void AdjustZoom(float yOffset, const glm::vec2& mousePos, const glm::vec2& windowSize);

    const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    glm::vec2 GetMoveDirection() { return m_moveDirection; };
    float GetCameraSpeed() { return m_cameraSpeed; }
    bool GetAccelerate() { return m_accelerate; }

    void SetMoveDirection(glm::vec2 value) { m_moveDirection = value; }
    void ResetMoveDirection() { m_moveDirection = glm::vec2(0.0f, 0.0f); } 
    void SetAccelerate(bool value) { m_accelerate = value; }

    void Reset();

private:
    void UpdateViewMatrix();

    glm::mat4 m_viewMatrix;

    glm::vec2 m_position;
    glm::vec2 m_moveDirection;

    float m_cameraSpeed, m_zoom;
    bool m_accelerate;
};