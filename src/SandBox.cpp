#include "SandBox.h"

#include <exception>

unsigned int PIXEL_SIZE = 256; // temp

SandBox::SandBox()
    :  m_shader("../res/shaders/rect.vs", "../res/shaders/rect.fs"), m_rect(glm::vec2(10.0f, 10.0f), 20.0f, 20.0f, m_shader),
    m_pixelVertices(CreatePixelData()), m_pixelBatchRenderer(m_pixelVertices)
{
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        std::cout << "Current working directory:" << buffer << std::endl;
    }
    else {
        std::cerr << "getcwd() error, cannot figure out the current working directory!";
    }

    // pixelsMatrix.reserve(PIXEL_SIZE);
    // for (int y = 0; y < PIXEL_SIZE; y++) {
    //     pixelsMatrix.emplace_back();
    //     pixelsMatrix.back().reserve(PIXEL_SIZE);

    //     for (int x = 0; x < PIXEL_SIZE; x++) {
    //         float color = (x % 100) / 100.0f;
            

    //         pixels.push_back(pix);
    //     }

    //     pixelsMatrix.push_back(pixels);
    // }
}

SandBox::~SandBox()
{
}

void SandBox::Draw() {
    m_pixelBatchRenderer.Draw(WINDOW_WIDTH, WINDOW_HEIGHT);
}

std::vector<PixelVertex> SandBox::CreatePixelData()
{
    PerlinNoise::Init(10);

    std::vector<PixelVertex> vertices;
    vertices.reserve(PIXEL_SIZE * PIXEL_SIZE);

    double frequency = 0.05f;

    for (int y = 0; y < PIXEL_SIZE; y++) {
        for (int x = 0; x < PIXEL_SIZE; x++) {

            double noiseValue = PerlinNoise::FractalBrownianMotion(x, y, 8);

            float colorValue = static_cast<float>((noiseValue + 1.0f) / 2.0f);

            glm::vec2 pos(x, y);
            glm::vec4 color(colorValue, colorValue, colorValue, 1.0f);

            vertices.emplace_back(pos, color);
        }
    }
    return vertices;
}