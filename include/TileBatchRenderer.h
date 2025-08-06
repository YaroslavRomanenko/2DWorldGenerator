#include <glm/glm.hpp>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

struct TileVertex {
    glm::vec2 position;
    glm::vec2 textureCoords;
    float textureId;
};

class TileBatchRenderer {
public:
    TileBatchRenderer(const std::vector<TileVertex>& tileVertices);

    void UpdateVerticesData(const std::vector<TileVertex>& tileVertices);

    void Draw(const Camera& camera, unsigned int windowWidth, unsigned int windowHeight);

private:
    std::vector<TileVertex> m_tileVertices;

    unsigned int m_VAO, m_VBO;
    Shader m_shader;

    Texture m_snowTex, m_stoneTex, m_grassTex, m_sandTex, m_shallowTex, m_deepWaterTex;
};