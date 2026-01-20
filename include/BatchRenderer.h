#pragma once

class BatchRenderer {
public:
    static void Init();
    static void Shutdown();
    static void BeginBatch();
    static void EndBatch();
    static void Flush();

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, unsigned int textureId);

private:
    static RendererData s_data;
};