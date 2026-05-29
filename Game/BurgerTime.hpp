#pragma once
#include "Game.hpp"
#include <memory>
namespace dae
{
    class Observer;
    class GameObject;
    class Achievements;
} // namespace dae
class BurgerTime : public Game
{
  public:
    BurgerTime();
    ~BurgerTime() override;
    void Initialize() override;
    void Destroy() override;

  private:
    void SetupKeybinds();
};
