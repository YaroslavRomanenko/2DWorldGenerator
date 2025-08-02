#pragma once

#include <glad/glad.h>

#include <iostream>

static void GLClearAllErrors() {
    while(glGetError() != GL_NO_ERROR) {

    }
}

static bool GLCheckErrorStatus() {
    while(GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error << std::endl;
        return true;
    }
    return false;
}

#define glCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus();