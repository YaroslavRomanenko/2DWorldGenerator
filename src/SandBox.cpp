#include "SandBox.h"

#include <exception>


SandBox::SandBox()
    :  m_shader("../res/shaders/rect.vs", "../res/shaders/rect.fs"), m_rect(glm::vec2(0.0f, 0.0f), 0.5f, 0.5f, m_shader)
{
}

void SandBox::Draw() {

   m_shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 0.0f));
    model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(50.0f, 50.0f, 1.0f));
    m_shader.SetMat4("model", model);

    glm::mat4 view = glm::mat4(1.0f);
    m_shader.SetMat4("view", view);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT), -1.0f, 1.0f);
    m_shader.SetMat4("projection", projection);

    m_rect.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_rect.Draw();
}