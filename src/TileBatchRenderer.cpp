#include "TileBatchRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

TileBatchRenderer::TileBatchRenderer(const std::vector<TileVertex>& tileVertices)
    : m_tileVertices(tileVertices), m_shader("../res/shaders/tile.vs", "../res/shaders/tile.fs"),
    m_snowTex("../res/assets/Snow.png"), m_stoneTex("../res/assets/Stone.png"), m_grassTex("../res/assets/Grass.png"),
    m_sandTex("../res/assets/Sand.png"), m_shallowTex("../res/assets/Shallow.png"), m_deepWaterTex("../res/assets/DeepWater.png")
{
    glCheck(glGenVertexArrays(1, &m_VAO);)
    glCheck(glBindVertexArray(m_VAO);)

    glCheck(glGenBuffers(1, &m_VBO);)
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO);)
    glCheck(glBufferData(GL_ARRAY_BUFFER, m_tileVertices.size() * sizeof(TileVertex), m_tileVertices.data(), GL_DYNAMIC_DRAW);)

    glCheck(glEnableVertexAttribArray(0);)
    glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TileVertex), (const void*)offsetof(TileVertex, position));)

    glCheck(glEnableVertexAttribArray(1);)
    glCheck(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TileVertex), (const void*)offsetof(TileVertex, textureCoords));)

    glCheck(glEnableVertexAttribArray(2);)
    glCheck(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(TileVertex), (const void*)offsetof(TileVertex, textureId));)

    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0);)
    glCheck(glBindVertexArray(0);)

    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);)
}

void TileBatchRenderer::UpdateVerticesData(const std::vector<TileVertex>& tileVertices)
{
    m_tileVertices = tileVertices;

    glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO);)
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_tileVertices.size() * sizeof(TileVertex), m_tileVertices.data());
}

void TileBatchRenderer::Draw(const Camera& camera, unsigned int windowWidth, unsigned int windowHeight) {
    m_shader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_deepWaterTex.GetTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_shallowTex.GetTextureId());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_sandTex.GetTextureId());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_grassTex.GetTextureId());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_stoneTex.GetTextureId());
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, m_snowTex.GetTextureId());

    glm::mat4 model = glm::mat4(1.0f);
    m_shader.SetMat4("model", model);

    glm::mat4 view = camera.GetViewMatrix();
    m_shader.SetMat4("view", view);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), -1.0f, 1.0f);
    m_shader.SetMat4("projection", projection);

    int samplers[6] = { 0, 1, 2, 3, 4, 5 };
    glUniform1iv(m_shader.GetUniformLocation("uTextures"), 6, samplers);

    glCheck(glBindVertexArray(m_VAO);)
    glDrawArrays(GL_TRIANGLES, 0, m_tileVertices.size());
    glCheck(glBindVertexArray(0);)
}