#include "BatchRenderer.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <array>

const int MaxQuadCount = 100;
const int MaxVertexCount = MaxQuadCount * 4;
const int MaxIndexCount = MaxQuadCount * 6;
const int MaxTextures = 32;

struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 texCoords;
    float texIndex;
};

struct RendererData {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    unsigned int whiteTexture = 0;
    unsigned int whiteTextureSlot = 0;

    unsigned int indexCount;

    Vertex* quadBuffer = nullptr;
    Vertex* quadBufferPtr = nullptr;

    std::array<unsigned int, MaxTextures> textureSlots;
    unsigned int textureSlotIndex = 1;
};

void BatchRenderer::Init() {
    s_data.quadBuffer = new Vertex[MaxVertexCount];

    glGenVertexArrays(1, &s_data.VAO);
    glBindVertexArray(s_data.VAO);

    glGenBuffers(1, &s_data.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_data.VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex));

    unsigned int indices[MaxIndexCount];
    unsigned int offset = 0;
    for (int i = 0; i < MaxIndexCount; i += 6) {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 3 + offset;
        indices[i + 4] = 4 + offset;
        indices[i + 5] = 5 + offset;

        offset += 4;
    }

    glGenBuffers(1, &s_data.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(GL_TEXTURE_2D, &s_data.whiteTexture);
    glBindTexture(GL_TEXTURE_2D, s_data.whiteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned int color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    s_data.textureSlots[0] = s_data.whiteTexture;
    for (int i = 1; i < MaxTextures; i++)
        s_data.textureSlots[i] = 0;
}

void BatchRenderer::Shutdown() {
    glDeleteVertexArrays(1, &s_data.VAO);
    glDeleteBuffers(1, &s_data.VBO);
    glDeleteBuffers(1, &s_data.EBO);

    delete[] s_data.quadBuffer;
}

void BatchRenderer::BeginBatch()
{
    s_data.quadBufferPtr = s_data.quadBuffer;
}

void BatchRenderer::EndBatch()
{
    unsigned int size = (unsigned int*)s_data.quadBufferPtr - (unsigned int*)s_data.quadBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, s_data.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_data.quadBuffer);
}

void BatchRenderer::Flush() 
{
    for (unsigned int i = 0; i < s_data.textureSlotIndex; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, s_data.textureSlots[i]);
    }

    glBindVertexArray(s_data.VAO);
    glDrawElements(GL_TRIANGLES, s_data.indexCount, GL_UNSIGNED_INT, nullptr);
}

void BatchRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    if (s_data.indexCount >= MaxIndexCount) {
        EndBatch();
        Flush();
        BeginBatch();
    }

    float textureIndex = 0.0f;

    s_data.quadBufferPtr->position = { position.x, position.y };
    s_data.quadBufferPtr->color = color;
    s_data.quadBufferPtr->texCoords = { 0.0f, 0.0f };
    s_data.quadBufferPtr->texIndex = textureIndex;
    s_data.quadBufferPtr++;

    s_data.quadBufferPtr->position = { position.x + size.x, position.y };
    s_data.quadBufferPtr->color = color;
    s_data.quadBufferPtr->texCoords = { 1.0f, 0.0f };
    s_data.quadBufferPtr->texIndex = textureIndex;
    s_data.quadBufferPtr++;

    s_data.quadBufferPtr->position = { position.x + size.x, position.y + size.y };
    s_data.quadBufferPtr->color = color;
    s_data.quadBufferPtr->texCoords = { 1.0f, 1.0f };
    s_data.quadBufferPtr->texIndex = textureIndex;
    s_data.quadBufferPtr++;

    s_data.quadBufferPtr->position = { position.x, position.y + size.y };
    s_data.quadBufferPtr->color = color;
    s_data.quadBufferPtr->texCoords = { 0.0f, 1.0f };
    s_data.quadBufferPtr->texIndex = textureIndex;
    s_data.quadBufferPtr++;

    s_data.indexCount += 6;
}

void BatchRenderer::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, unsigned int textureId)
{
    if (s_data.indexCount >= MaxIndexCount || s_data.textureSlotIndex > 31) {
        EndBatch();
        Flush();
        BeginBatch();
    }

    constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    float textureIndex = 0.0f;
    for (unsigned int i = 1; i < s_data.textureSlotIndex; i++) {
        if (s_data.textureSlots[i] == textureId) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        textureIndex = (float)s_data.textureSlotIndex;
        s_data.textureSlots[s_data.textureSlotIndex] = textureId;
        s_data.textureSlotIndex++;
    }

    s_data.quadBufferPtr->position = { position.x, position.y };
    s_data.quadBufferPtr->color = color;
    s_data.quadBufferPtr->texCoords = { 0.0f, 0.0f };
    s_data.quadBufferPtr->texIndex = textureIndex;
    s_data.quadBufferPtr++;

    s_data.quadBufferPtr->position = { position.x + size.x, position.y };
    s_data.quadBufferPtr->color = color;
    s_data.quadBufferPtr->texCoords = { 1.0f, 0.0f };
    s_data.quadBufferPtr->texIndex = textureIndex;
    s_data.quadBufferPtr++;

    s_data.quadBufferPtr->position = { position.x + size.x, position.y + size.y };
    s_data.quadBufferPtr->color = color;
    s_data.quadBufferPtr->texCoords = { 1.0f, 1.0f };
    s_data.quadBufferPtr->texIndex = textureIndex;
    s_data.quadBufferPtr++;

    s_data.quadBufferPtr->position = { position.x, position.y + size.y };
    s_data.quadBufferPtr->color = color;
    s_data.quadBufferPtr->texCoords = { 0.0f, 1.0f };
    s_data.quadBufferPtr->texIndex = textureIndex;
    s_data.quadBufferPtr++;

    s_data.indexCount += 6;
}

RendererData BatchRenderer::s_data;