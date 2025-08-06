#include "SandBox.h"

#include <exception>
#include <array>

unsigned int PIXEL_SIZE = 256; // temp
unsigned int TILES_SIZE = 256; // temp

SandBox::SandBox()
    :  m_shader("../res/shaders/tile.vs", "../res/shaders/tile.fs"), m_rect(glm::vec2(10.0f, 10.0f), 20.0f, 20.0f, m_shader),
    m_pixelBatchRenderer(CreatePerlinNoisePixelData()), m_tileBatchRenderer(CreatePerlinNoiseTileData())
{
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        std::cout << "Current working directory:" << buffer << std::endl;
    }
    else {
        std::cerr << "getcwd() error, cannot figure out the current working directory!";
    }
}

SandBox::~SandBox()
{
}

void SandBox::RegenerateMap(int seed, double amplitude, double frequency, MapType type)
{
    m_mapType = type;
    if (m_mapType == DefaultMap || m_mapType == ColoredMap) {
        std::vector<PixelVertex> pixelVertices = CreatePerlinNoisePixelData(seed, amplitude, frequency, type);
        m_pixelBatchRenderer.UpdateVerticesData(pixelVertices);
    }
    else {
        std::vector<TileVertex> tileVertices = CreatePerlinNoiseTileData(seed, amplitude, frequency, type);
        m_tileBatchRenderer.UpdateVerticesData(tileVertices);
    }

}

void SandBox::Update(float dt)
{
}

void SandBox::Draw() {
    if (m_mapType == DefaultMap || m_mapType == ColoredMap)
        m_pixelBatchRenderer.Draw(WINDOW_WIDTH, WINDOW_HEIGHT);
    else
        m_tileBatchRenderer.Draw(WINDOW_WIDTH, WINDOW_HEIGHT);
}

std::vector<PixelVertex> SandBox::CreatePerlinNoisePixelData(int seed, double amplitude, double frequency, MapType type)
{
    PerlinNoise::Init(seed);

    std::vector<PixelVertex> vertices;
    vertices.reserve(PIXEL_SIZE * PIXEL_SIZE);

    for (int y = 0; y < PIXEL_SIZE; y++) {
        for (int x = 0; x < PIXEL_SIZE; x++) {

            double noiseValue = PerlinNoise::FractalBrownianMotion(x, y, 8, amplitude, frequency);

            float colorValue = static_cast<float>((noiseValue + 1.0f) / 2.0f);
 
            glm::vec2 pos(x, y);
            glm::vec4 color(colorValue, colorValue, colorValue, 1.0f);
            
            if (type == ColoredMap) {
                if (colorValue <= 0.2f) {
                    color.r = 0.0f; color.g = 0.0f; color.b = 0.9f;
                }
                else if (colorValue <= 0.4f) {
                    color.r = 0.5f; color.g = 0.5f; color.b = 1.0f;
                }
                else if (colorValue <= 0.5f) {
                    color.r = 0.96f; color.g = 0.86f; color.b = 0.74f;
                }
                else if (colorValue <= 0.8f) {
                    color.r = 0.0f, color.g = 0.7f; color.b = 0.0f;
                }
                else {
                    color.r = 1.0f; color.g = 1.0f; color.b = 1.0f;
                }
            }
            
            vertices.emplace_back(pos, color);
        }
    }

    
    return vertices;
}

std::vector<TileVertex> SandBox::CreatePerlinNoiseTileData(int seed, double amplitude, double frequency, MapType type) {
    PerlinNoise::Init(seed);

    std::vector<TileVertex> vertices;
    vertices.reserve(TILES_SIZE * TILES_SIZE * 6);

    for (int y = 0; y < TILES_SIZE; y++) {
        for (int x = 0; x < TILES_SIZE; x++) { 
            double noiseValue = PerlinNoise::FractalBrownianMotion(x, y, 8, amplitude, frequency);

            float colorValue = static_cast<float>((noiseValue + 1.0f) / 2.0f);

            float width = 16.0f, height = 16.0f;
            float xPos = x * width, yPos = y * height; 
            glm::vec2 bottomLeftVertexPos (xPos,         yPos);
            glm::vec2 bottomRightVertexPos(xPos + width, yPos);
            glm::vec2 TopLeftVertexPos    (xPos,         yPos + 16.0f);
            glm::vec2 TopRightVertexPos   (xPos + width, yPos + 16.0f);

            float textureId;

            if (colorValue <= 0.2f) {
                textureId = 0.0f;
            }
            else if (colorValue <= 0.4f) {
                textureId = 1.0f;
            }
            else if (colorValue <= 0.5f) {
                textureId = 2.0f;
            }
            else if (colorValue <= 0.8f) {
                textureId = 3.0f;
            }
            else if (colorValue <= 0.9f) {
                textureId = 4.0f;
            }
            else {
                textureId = 5.0f;
            }

            vertices.push_back({ bottomLeftVertexPos, glm::vec2(0.0f, 0.0f), textureId });
            vertices.push_back({ bottomRightVertexPos, glm::vec2(1.0f, 0.0f), textureId });
            vertices.push_back({ TopLeftVertexPos, glm::vec2(0.0f, 1.0f), textureId });

            vertices.push_back({ bottomRightVertexPos, glm::vec2(1.0f, 0.0f), textureId });
            vertices.push_back({ TopLeftVertexPos, glm::vec2(0.0f, 1.0f), textureId });
            vertices.push_back({ TopRightVertexPos, glm::vec2(1.0f, 1.0f), textureId });
        }
    }

    return vertices;
}