#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <unistd.h>
#include <vector>
#include <memory>

#include "Shader.h"
#include "PerlinNoise.h"
#include "PixelBatchRenderer.h"
#include "TileBatchRenderer.h"
#include "Camera.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

enum MapType {
    DefaultMap,
    ColoredMap,
    TiledMap
};

class SandBox {
public:
    SandBox();
    ~SandBox();

    void RegenerateMap(int seed, double amplitude, double frequency, MapType type);

    void Start();

    void Input();
    void Update(float dt);
    void Draw();

private:
    GLFWwindow* m_window;

    std::unique_ptr<PixelBatchRenderer> m_pixelBatchRenderer;
    std::unique_ptr<TileBatchRenderer> m_tileBatchRenderer;
    std::unique_ptr<Camera> m_camera;
    
    MapType m_mapType;

    std::vector<PixelVertex> CreatePerlinNoisePixelData(int seed = 1, double amplitude = 1.0, double frequency = 0.005, MapType type = DefaultMap);
    std::vector<TileVertex> CreatePerlinNoiseTileData(int seed = 1, double amplitude = 1.0, double frequency = 0.005, MapType type = DefaultMap);

    void SetUpGLFW();
    void SetUpGLAD();
    void SetUpImGui();
    
    void ProcessInput();

    static void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
};