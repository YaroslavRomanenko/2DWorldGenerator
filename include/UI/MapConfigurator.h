#include <imgui.h>

#include <string>

class MapConfigurator {
public:
    static void Draw() {
        ImGui::Begin("Debug");
        ImGui::InputText("Seed", m_buffer, sizeof(m_buffer));

        ImGui::SliderInt("Octaves", &m_octaves, 1, 8);
        ImGui::SliderFloat("Amplitude", &m_amplitude, 0.0f, 2.0f);
        ImGui::SliderFloat("Frequency", &m_frequency, 0.0f, 0.05f);
    
        ImGui::Separator();
        ImGui::RadioButton("Default Perlin Noise", &m_mapOption, 0);
        ImGui::RadioButton("Colored Map", &m_mapOption, 1);
        ImGui::RadioButton("TiledMap", &m_mapOption, 2);

        if (ImGui::Button("Regenerate")) {
            m_regenerate = true;
        }
        
        ImGui::Separator();
        ImGui::Text("%s", textToShow.c_str());

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

    static int GetOctaves() { return m_octaves; }
    static double GetAmplitude() { return m_amplitude; };
    static double GetFrequency() { return m_frequency; };
    static int GetMapType() { return m_mapOption; }

    static void SetTextToShow(const std::string& value) { textToShow = value; }

private:
    static std::string textToShow;
    static char m_buffer[256];
    static bool m_regenerate;

    static int m_octaves;
    static float m_amplitude;
    static float m_frequency;
    
    static int m_mapOption;
};

std::string MapConfigurator::textToShow = "";
char MapConfigurator::m_buffer[256] = {};
bool MapConfigurator::m_regenerate = false;

int MapConfigurator::m_octaves = 8;
float MapConfigurator::m_amplitude = 1.0f;
float MapConfigurator::m_frequency = 0.005f;

int MapConfigurator::m_mapOption = 0;