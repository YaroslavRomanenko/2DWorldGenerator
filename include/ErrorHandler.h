#pragma once

#include <glad/glad.h>

#include <iostream>

static void GLClearAllErrors() {
    while(glGetError() != GL_NO_ERROR) {

    }
}

static bool GLCheckErrorStatus(const char* function, int line) {
    while(GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error << "\tLine: " << line << "\tfunction: " << function << std::endl;
        return true;
    }
    return false;
}

#define glCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x, __LINE__);