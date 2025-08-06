#include "SandBox.h"

#include <stdexcept>
#include <array>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "UI/MapConfigurator.h"

unsigned int PIXEL_SIZE = 256; // temp
unsigned int TILES_SIZE = 256; // temp

const std::string TITLE = "OpenGLSandBox";

SandBox::SandBox()
{
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        std::cout << "Current working directory:" << buffer << std::endl;
    }
    else {
        std::cerr << "getcwd() error, cannot figure out the current working directory!";
    }

    SetUpGLFW();
    SetUpGLAD();
    SetUpImGui();

    m_mapType = DefaultMap;

    m_pixelBatchRenderer = std::make_unique<PixelBatchRenderer>(CreatePerlinNoisePixelData());
    m_tileBatchRenderer = std::make_unique<TileBatchRenderer>(CreatePerlinNoiseTileData());
    m_camera = std::make_unique<Camera>();
}

SandBox::~SandBox()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void SandBox::RegenerateMap(int seed, int octaves, double amplitude, double frequency, MapType type)
{
    m_mapType = type;

    m_camera->Reset();

    if (m_mapType == DefaultMap || m_mapType == ColoredMap) {
        std::vector<PixelVertex> pixelVertices = CreatePerlinNoisePixelData(seed, octaves, amplitude, frequency, type);
        m_pixelBatchRenderer->UpdateVerticesData(pixelVertices);
    }
    else {
        std::vector<TileVertex> tileVertices = CreatePerlinNoiseTileData(seed, octaves, amplitude, frequency, type);
        m_tileBatchRenderer->UpdateVerticesData(tileVertices);
    }

}

void SandBox::Start()
{
    float lastFrameTime = 0.0f;
    int frameCount = 0;
    double previousFPSTime = glfwGetTime();

    float deltaTime = 0.0f;

    char buffer[256] = {};


    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        ProcessInput();

        double currentFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

        frameCount++;
        
        if (currentFrameTime - previousFPSTime >= 1.0) {
            std::string fpsText = "FPS: " + std::to_string(frameCount);
            MapConfigurator::SetTextToShow(fpsText);

            frameCount = 0;
            previousFPSTime = currentFrameTime;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        MapConfigurator::Draw();
        if (MapConfigurator::RegenerateButtonPressed()) {
            int seed = MapConfigurator::GetSeed();

            int octaves = MapConfigurator::GetOctaves();
            float amplitude = MapConfigurator::GetAmplitude();
            float frequency = MapConfigurator::GetFrequency();

            MapType type = static_cast<MapType>(MapConfigurator::GetMapType());

            RegenerateMap(seed, octaves, amplitude, frequency, type);
            MapConfigurator::ResetRegenerateButtonPressed();
        }
        
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Update(deltaTime);
        Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        
        glfwSwapBuffers(m_window);
    }
}

void SandBox::Update(float dt)
{
    if (glm::length(m_camera->GetMoveDirection()) > 0.0f) {
        glm::vec2 direction = glm::normalize(m_camera->GetMoveDirection());

        float cameraSpeed = m_camera->GetCameraSpeed();
        if (m_camera->GetAccelerate()) cameraSpeed *= 2;

        glm::vec2 velocity = direction * cameraSpeed * dt;

        m_camera->Move(velocity);
    } 
}

void SandBox::Draw() {
    if (m_mapType == DefaultMap || m_mapType == ColoredMap)
        m_pixelBatchRenderer->Draw(WINDOW_WIDTH, WINDOW_HEIGHT);
    else
        m_tileBatchRenderer->Draw(*m_camera, WINDOW_WIDTH, WINDOW_HEIGHT);
}

std::vector<PixelVertex> SandBox::CreatePerlinNoisePixelData(int seed, int octaves, double amplitude, double frequency, MapType type)
{
    PerlinNoise::Init(seed);

    std::vector<PixelVertex> vertices;
    vertices.reserve(PIXEL_SIZE * PIXEL_SIZE);

    for (int y = 0; y < PIXEL_SIZE; y++) {
        for (int x = 0; x < PIXEL_SIZE; x++) {

            double noiseValue = PerlinNoise::FractalBrownianMotion(x, y, octaves, amplitude, frequency);

            float colorValue = static_cast<float>((noiseValue + 1.0f) / 2.0f);
 
            glm::vec2 pos(x, y);
            glm::vec4 color(colorValue, colorValue, colorValue, 1.0f);
            
            if (type == ColoredMap) {
                if (colorValue <= 0.3f) {
                    color.r = 0.0f; color.g = 0.0f; color.b = 0.9f;
                }
                else if (colorValue <= 0.4f) {
                    color.r = 0.5f; color.g = 0.5f; color.b = 1.0f;
                }
                else if (colorValue <= 0.45f) {
                    color.r = 0.96f; color.g = 0.86f; color.b = 0.74f;
                }
                else if (colorValue <= 0.75f) {
                    color.r = 0.0f, color.g = 0.7f; color.b = 0.0f;
                }
                else if (colorValue <= 0.9f) {
                    color.r = 0.5f; color.g = 0.5f; color.b = 0.5f;
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

std::vector<TileVertex> SandBox::CreatePerlinNoiseTileData(int seed, int octaves, double amplitude, double frequency, MapType type) {
    PerlinNoise::Init(seed);

    std::vector<TileVertex> vertices;
    vertices.reserve(TILES_SIZE * TILES_SIZE * 6);

    for (int y = 0; y < TILES_SIZE; y++) {
        for (int x = 0; x < TILES_SIZE; x++) { 
            double noiseValue = PerlinNoise::FractalBrownianMotion(x, y, octaves, amplitude, frequency);

            float colorValue = static_cast<float>((noiseValue + 1.0f) / 2.0f);

            float width = 16.0f, height = 16.0f;
            float xPos = x * width, yPos = y * height; 
            glm::vec2 bottomLeftVertexPos (xPos,         yPos);
            glm::vec2 bottomRightVertexPos(xPos + width, yPos);
            glm::vec2 TopLeftVertexPos    (xPos,         yPos + 16.0f);
            glm::vec2 TopRightVertexPos   (xPos + width, yPos + 16.0f);

            float textureId;

            if (colorValue <= 0.3f) {
                textureId = 0.0f;
            }
            else if (colorValue <= 0.4f) {
                textureId = 1.0f;
            }
            else if (colorValue <= 0.45f) {
                textureId = 2.0f;
            }
            else if (colorValue <= 0.75f) {
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

void SandBox::SetUpGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE.c_str(), NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetWindowUserPointer(m_window, this);

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, frame_buffer_size_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
}

void SandBox::SetUpGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void SandBox::SetUpImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 420");
}

void SandBox::ProcessInput()
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    m_camera->ResetMoveDirection();
    m_camera->SetAccelerate(false);

    glm::vec2 moveDirection(0.0f);

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        moveDirection.y += 1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        moveDirection.y -= 1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        moveDirection.x += 1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        moveDirection.x -= 1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        m_camera->SetAccelerate(true);

    m_camera->SetMoveDirection(moveDirection);
}

void SandBox::frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void SandBox::ScrollCallbackImpl(double yOffset)
{
    double xPos, yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    yPos = WINDOW_HEIGHT - yPos;
    glm::vec2 mousePos = glm::vec2(static_cast<float>(xPos), static_cast<float>(yPos));

    m_camera->AdjustZoom(static_cast<float>(yOffset), mousePos, glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
}

void SandBox::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    SandBox* instance = static_cast<SandBox*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->ScrollCallbackImpl(yOffset);
    }
}
