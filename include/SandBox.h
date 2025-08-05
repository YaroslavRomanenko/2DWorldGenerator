#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <unistd.h>
#include <vector>

#include "Shader.h"
#include "Rectangle.h"
#include "PerlinNoise.h"
#include "PixelBatchRenderer.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

class SandBox {
public:
    SandBox();
    ~SandBox();

    void RegenerateMap(int seed, double amplitude = 1.0, double frequency = 0.005);

    void Input();
    void Update(float dt);
    void Draw();

private:
    GLFWwindow* window;
    Shader m_shader;
    Rectangle m_rect;

    PixelBatchRenderer m_pixelBatchRenderer;

    std::vector<PixelVertex> CreatePerlinNoisePixelData(int seed, double amplitude = 1.0, double frequency = 0.005);
};