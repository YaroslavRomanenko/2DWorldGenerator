#include <imgui.h>

#include <string>

class MapConfigurator {
public:
    static void Draw() {
        ImGui::Begin("Debug");
        ImGui::InputText("Seed", m_buffer, sizeof(m_buffer));

        if (ImGui::Button("Regenerate")) {
            m_regenerate = true;
        }

        ImGui::End();
    }

    static bool RegenerateButtonPressed() { return m_regenerate; }
    static void ResetRegenerateButtonPressed() { m_regenerate = false; }

    static int GetSeed() { 
        if (strlen(m_buffer) == 0) {
            m_buffer[0] = '1'; 
            m_buffer[1] = '\0';
            return 1;
        }
        return std::stoi(m_buffer); 
    }

private:
    static char m_buffer[256];
    static bool m_regenerate;
};

char MapConfigurator::m_buffer[256] = {};
bool MapConfigurator::m_regenerate = false;