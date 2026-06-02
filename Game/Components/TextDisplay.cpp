#include "Font.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "TextDisplay.hpp"
#include "Texture2DDisplay.hpp"
#include "Transform.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>

dae::TextDisplay::TextDisplay(
    GameObject &owner, const std::string &text, std::shared_ptr<Font> font, const SDL_Color &color)
    : Component(owner),
      m_needsUpdate(true),
      m_text(text),
      m_color(color),
      m_font(std::move(font)),
      m_TextTexture(nullptr) {
    Update();
}

void dae::TextDisplay::Update() {
    if (m_needsUpdate) {
        const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
        if (surf == nullptr) {
            throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
        }
        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        if (texture == nullptr) {
            throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
        }
        SDL_DestroySurface(surf);
        if (m_TextTexture == nullptr) {
            m_TextTexture = GetOwner()->GetComponent<Texture2DDisplay>();
        }
        if (m_TextTexture == nullptr) {
            GetOwner()->AddComponent<Texture2DDisplay>(texture);
            m_TextTexture = GetOwner()->GetComponent<Texture2DDisplay>();
        } else {
            m_TextTexture->SetSDLTexture(texture);
        }
        m_needsUpdate = false;
    }
}

void dae::TextDisplay::SetText(const std::string &text) {
    m_text = text;
    m_needsUpdate = true;
    Update();
}

void dae::TextDisplay::SetColor(const SDL_Color &color) {
    m_color = color;
    m_needsUpdate = true;
    Update();
}

// std::shared_ptr<dae::Texture2DDisplay> dae::TextComponent::GetTexture() const
//{
//	return m_TextTexture;
// }
