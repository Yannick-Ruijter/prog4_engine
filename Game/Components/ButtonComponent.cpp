#include "ButtonComponent.hpp"
#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "TextComponent.hpp"
using namespace dae;

ButtonComponent::ButtonComponent(GameObject &owner, SDL_Color focusColor, SDL_Color idleColor)
    : Component(owner),
      m_TextComponent{owner.GetComponent<TextComponent>()},
      m_FocusColor{focusColor},
      m_IdleColor{idleColor}
{
    assert(m_TextComponent != nullptr && "A buttonComponent needs a textcomponent (created before buttoncomponent)");
    m_TextComponent->SetColor(idleColor);
}

void dae::ButtonComponent::GainFocus()
{
    m_TextComponent->SetColor(m_FocusColor);
}

void dae::ButtonComponent::LoseFocus()
{
    m_TextComponent->SetColor(m_IdleColor);
}

void dae::ButtonComponent::AddNeighbor(Direction dir, ButtonComponent *button)
{
    m_Neighbors[dir] = button;
}

void dae::ButtonComponent::Execute()
{
}

ButtonComponent *dae::ButtonComponent::GetNeighbor(Direction dir)
{
    if (!m_Neighbors.contains(dir))
        return this;
    return m_Neighbors.at(dir);
}
