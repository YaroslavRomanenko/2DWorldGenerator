#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "Rectangle.h"

class SandBox {
private:
    GLFWwindow* window;
    Shader m_shader;
    Rectangle m_rect;

public:
    SandBox();
    Rectangle& GetRectRef() { return m_rect; }
    Shader& GetShaderRef() { return m_shader; }

    void Input();
    void Update(float dt);
    void Draw();
};