#include "SandBox.h"

#include <exception>

unsigned int PIXEL_SIZE = 256; // temp

SandBox::SandBox()
    :  m_shader("../res/shaders/rect.vs", "../res/shaders/rect.fs"), m_rect(glm::vec2(10.0f, 10.0f), 20.0f, 20.0f, m_shader),
    m_pixelBatchRenderer(CreatePerlinNoisePixelData())
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
    std::vector<PixelVertex> pixelVertices = CreatePerlinNoisePixelData(seed, amplitude, frequency, type);
    m_pixelBatchRenderer.UpdateVerticesData(pixelVertices);
}

void SandBox::Draw() {
    m_pixelBatchRenderer.Draw(WINDOW_WIDTH, WINDOW_HEIGHT);
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