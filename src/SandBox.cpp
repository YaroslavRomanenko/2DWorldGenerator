#include "SandBox.h"

#include <exception>


SandBox::SandBox()
    :  m_shader("../res/shaders/rect.vs", "../res/shaders/rect.fs"), m_rect(glm::vec2(0.0f, 0.0f), 0.5f, 0.5f, m_shader)
{
}

void SandBox::Draw() {
    m_rect.Draw();
}