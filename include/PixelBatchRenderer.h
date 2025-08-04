#include <glm/glm.hpp>

#include <vector>
#include <cstring>

#include "Shader.h"

struct PixelVertex {
    PixelVertex(const glm::vec2& pos, const glm::vec4& col) 
        : position(pos), color(col)
    { }

    glm::vec2 position;
    glm::vec4 color;
};

class PixelBatchRenderer {
public:
    PixelBatchRenderer(const std::vector<PixelVertex>& pixelVertices);

    void Draw(unsigned int windowWidth, unsigned int windowHeight);

private:
    const std::vector<PixelVertex>& m_pixelVertices;

    unsigned int m_VAO;
    Shader m_shader;
};