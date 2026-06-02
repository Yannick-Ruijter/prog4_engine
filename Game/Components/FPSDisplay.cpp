#include "FPSDisplay.hpp"
#include "GameObject.hpp"
#include "TextDisplay.hpp"
#include "TimeManager.hpp"
#include <format>
#include <memory>

dae::FpsDisplay ::FpsDisplay(GameObject &owner) : Component(owner), m_TimeManager{&TimeManager::GetInstance()} {
}

void dae::FpsDisplay::Update() {
    float currentFPS{1 / m_TimeManager->GetDeltaTime()};
    if (currentFPS - m_LastFps > 0.1f || currentFPS - m_LastFps < -0.1f || m_LastFps == 0.0f) {
        if (m_TextComponent == nullptr)
            m_TextComponent = GetOwner()->GetComponent<TextDisplay>();
        if (m_TextComponent == nullptr)
            return;
        m_LastFps = currentFPS;
        std::string fps = std::to_string(m_LastFps);
        fps.resize(4);
        fps += " FPS";
        m_TextComponent->SetText(fps);
    }
}

float dae::FpsDisplay::GetDeltaTime() const {
    return m_LastFps;
}
