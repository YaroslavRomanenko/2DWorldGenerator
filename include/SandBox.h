#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Shader.h"
#include "Rectangle.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

class SandBox {
private:
    GLFWwindow* window;
    Shader m_shader;
    Rectangle m_rect;

public:
    SandBox();

    void Input();
    void Update(float dt);
    void Draw();
};