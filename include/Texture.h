#pragma once

#include <string>

class Texture {
public:
    Texture(const std::string& imagePath);
    ~Texture();

    unsigned int GetTextureId() { return m_texture; }

private:
    unsigned int m_texture;
};