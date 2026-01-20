#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vsFilePath, const std::string& fsFilePath)
{
    std::string vsSource = ReadShaderFile(vsFilePath);
    std::string fsSource = ReadShaderFile(fsFilePath);

    unsigned int vertexShader = CompileShader(vsSource, VERTEX);
    CheckForErrors(vertexShader, "VERTEX");
    unsigned int fragmentShader = CompileShader(fsSource, FRAGMENT);
    CheckForErrors(fragmentShader, "FRAGMENT");

    m_shaderProgram = glCreateProgram();
    glCheck(glAttachShader(m_shaderProgram, vertexShader);)
    glCheck(glAttachShader(m_shaderProgram, fragmentShader);)
    glCheck(glLinkProgram(m_shaderProgram);)

    CheckForErrors(m_shaderProgram, "PROGRAM");

    glCheck(glDeleteShader(vertexShader);)
    glCheck(glDeleteShader(fragmentShader);)
}

void Shader::Use()
{
    glCheck(glUseProgram(m_shaderProgram);)
}

int Shader::GetUniformLocation(const std::string & name)
{
    return glGetUniformLocation(m_shaderProgram, name.c_str());
}

void Shader::SetVec4(const std::string& name, const glm::vec4 & value)
{
    glCheck(glUniform4fv(GetUniformLocation(name), 1, &value[0]);)
}

void Shader::SetVec4(const std::string & name, float x, float y, float z, float w)
{
    glCheck(glUniform4f(GetUniformLocation(name), x, y, z, w);)
}

void Shader::SetMat4(const std::string & name, const glm::mat4 & value)
{
    glCheck(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);)
}

std::string Shader::ReadShaderFile(const std::string& shaderFilePath)
{
    std::stringstream source;
    std::ifstream file(shaderFilePath);
    if (!file.is_open()) {
        std::cout << "Failed to open the file in path: " << shaderFilePath << std::endl; 
    }
    source << file.rdbuf();
    file.close();

    std::string result = source.str();
    return result;
}

unsigned int Shader::CompileShader(const std::string& source, ShaderType shaderType)
{
    unsigned int shader;
    const char* pSource = source.c_str();

    if (shaderType == VERTEX)
        shader = glCreateShader(GL_VERTEX_SHADER);
    else if (shaderType == FRAGMENT)
        shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader, 1, &pSource, NULL);
    glCompileShader(shader);

    return shader;
}

void Shader::CheckForErrors(unsigned int object, const std::string& type)
{
    unsigned int infoLogSize = 1024;
    int success;
    char infoLog[infoLogSize];

    if (!(type == "PROGRAM")) {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, infoLogSize, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::COMPILATION::FAILED\n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, infoLogSize, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::LINKING::FAILED\n" << infoLog << std::endl;
        }
    }
}