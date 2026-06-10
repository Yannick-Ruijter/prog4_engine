#pragma once
#include "Component.hpp"
#include "Observer.hpp"
#include <string>
#include <vector>
namespace dae {
class GameObject;

class LivesDisplay : public Component, public Observer {
public:
  LivesDisplay(GameObject &gameObject, std::string const &imagePath, int lives);
  virtual void Notify(EventId event, GameObject *source) override;

private:
  void DecrementLives();
  int m_Lives{};
  GameObject *m_LifeText{nullptr};
  GameObject *m_LifeImage{nullptr};
};
} // namespace dae
