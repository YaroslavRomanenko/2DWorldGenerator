#include "Rectangle.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Rectangle::Rectangle(const glm::vec2& pos, float width, float height, const Shader& shader)
    : m_position(pos), m_width(width), m_height(height), m_shader(shader), m_color(1.0f)
{
    unsigned int VBO, EBO;

    float vertices[] = {
        pos.x,         pos.y,
        pos.x + width, pos.y,
        pos.x,         pos.y + height,
        pos.x + width, pos.y + height
    };
    unsigned int indeces[] = {
        0, 1, 2,
        1, 2, 3
    };

    glCheck(glGenVertexArrays(1, &m_VAO);)
    glCheck(glBindVertexArray(m_VAO);)

    glCheck(glGenBuffers(1, &VBO);)
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO);)
    glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);)

    glCheck(glGenBuffers(1, &EBO);)
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);)
    glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);)

    glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);)
    glCheck(glEnableVertexAttribArray(0);)

    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0);)
    glCheck(glBindVertexArray(0);)

    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);)
}

void Rectangle::Draw()
{
    m_shader.Use();
    m_shader.SetVec4("uColor", m_color);
    glCheck(glBindVertexArray(m_VAO);)
    glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);)
    glCheck(glBindVertexArray(0);)
}
