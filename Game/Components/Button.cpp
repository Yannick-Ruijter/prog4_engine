#include "Button.hpp"
#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "TextDisplay.hpp"
using namespace dae;

Button::Button(
    GameObject &owner, std::function<void()> OnExecute, SDL_Color focusColor, SDL_Color idleColor)
    : Component(owner),
      m_OnExecute{OnExecute},
      m_TextComponent{owner.GetComponent<TextDisplay>()},
      m_FocusColor{focusColor},
      m_IdleColor{idleColor} {
    assert(m_TextComponent != nullptr && "A buttonComponent needs a textcomponent (created before buttoncomponent)");
    m_TextComponent->SetColor(idleColor);
}

void dae::Button::GainFocus() {
    m_TextComponent->SetColor(m_FocusColor);
}

void dae::Button::LoseFocus() {
    m_TextComponent->SetColor(m_IdleColor);
}

void dae::Button::AddNeighbor(Direction dir, Button *button) {
    m_Neighbors[dir] = button;
}

void dae::Button::Execute() {
    m_OnExecute();
}

Button *dae::Button::GetNeighbor(Direction dir) {
    if (!m_Neighbors.contains(dir))
        return this;
    return m_Neighbors.at(dir);
}
