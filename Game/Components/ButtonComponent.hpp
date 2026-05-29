#pragma once

#include "Component.hpp"
#include <SDL3/SDL_pixels.h>
#include <string>
#include <unordered_map>
namespace dae
{
    class TextComponent;
    enum class Direction;
    class ButtonComponent : public Component
    {
      public:
        ButtonComponent(GameObject &owner, SDL_Color focusColor, SDL_Color idleColor);
        void GainFocus();
        void LoseFocus();
        void AddNeighbor(Direction dir, ButtonComponent *button);
        void Execute();
        ButtonComponent *GetNeighbor(Direction dir);

      private:
        std::unordered_map<Direction, ButtonComponent *> m_Neighbors{};
        TextComponent *m_TextComponent;
        SDL_Color m_FocusColor;
        SDL_Color m_IdleColor;
    };
} // namespace dae
