#include "PixelBatchRenderer.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>

#include "ErrorHandler.h"

PixelBatchRenderer::PixelBatchRenderer(const std::vector<PixelVertex>& pixelVertices)
    : m_pixelVertices(pixelVertices), m_shader("../res/shaders/pixel.vs", "../res/shaders/pixel.fs")
{
    unsigned int VBO;

    glCheck(glGenVertexArrays(1, &m_VAO);)
    glCheck(glBindVertexArray(m_VAO);)

    glCheck(glGenBuffers(1, &VBO);)
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO);)
    glCheck(glBufferData(GL_ARRAY_BUFFER, m_pixelVertices.size() * sizeof(m_pixelVertices), m_pixelVertices.data(), GL_STATIC_DRAW);)

    glCheck(glEnableVertexAttribArray(0);)
    glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PixelVertex), (void*)offsetof(PixelVertex, position));)

    glCheck(glEnableVertexAttribArray(1);)
    glCheck(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PixelVertex), (void*)offsetof(PixelVertex, color));)

    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0);)
    glCheck(glBindVertexArray(0);)

    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);)
}

void PixelBatchRenderer::Draw(unsigned int windowWidth, unsigned int windowHeight) {
    m_shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((windowWidth / 2) - (512 / 4), (windowHeight / 2) - (512 / 4), 0.0f));
    m_shader.SetMat4("model", model);

    glm::mat4 view = glm::mat4(1.0f);
    m_shader.SetMat4("view", view);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), -1.0f, 1.0f);
    m_shader.SetMat4("projection", projection);

    glCheck(glBindVertexArray(m_VAO);)
    glCheck(glDrawArrays(GL_POINTS, 0, m_pixelVertices.size());)
    glCheck(glBindVertexArray(0);)
}