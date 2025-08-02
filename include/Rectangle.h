#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "ErrorHandler.h"

class Rectangle {
public:
    Rectangle(const glm::vec2& pos, float width, float height, const Shader& shader);

    void Draw();

    void SetColor(const glm::vec4& color) { m_color = color; }
private:
    unsigned int m_VAO;

    glm::vec2 m_position;
    glm::vec4 m_color;
    float m_width, m_height;

    Shader m_shader;
};