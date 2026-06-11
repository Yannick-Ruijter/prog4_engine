#pragma once

#include "Component.hpp"
#include <SDL3/SDL_pixels.h>
#include <functional>
#include <string>
#include <unordered_map>
namespace dae {
class TextDisplay;
enum class Direction;
class Button : public Component {
public:
  Button(GameObject &owner, std::function<void()> OnExecute,
         SDL_Color focusColor, SDL_Color idleColor);
  void GainFocus();
  void LoseFocus();
  void AddNeighbor(Direction dir, Button *button);
  void Execute();
  Button *GetNeighbor(Direction dir);

private:
  std::function<void()> m_OnExecute;
  std::unordered_map<Direction, Button *> m_Neighbors{};
  TextDisplay *m_TextComponent;
  SDL_Color m_FocusColor;
  SDL_Color m_IdleColor;
};
} // namespace dae
