#pragma once

#include <string>
#include <glm/glm.hpp>

#include "ErrorHandler.h"

enum ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader {
public:
    Shader(const std::string& vsFilePath, const std::string& fsFilePath);

    void Use();

    void SetVec4(const std::string& name, const glm::vec4& value);
    void SetVec4(const std::string& name, float x, float y, float z, float w);

    void SetMat4(const std::string& name, const glm::mat4& value);
private:
    unsigned int m_shaderProgram;

    std::string ReadShaderFile(const std::string& shaderFilePath);
    unsigned int CompileShader(const std::string& source, ShaderType shaderType);
    void CheckForErrors(unsigned int shader, const std::string& type);
};