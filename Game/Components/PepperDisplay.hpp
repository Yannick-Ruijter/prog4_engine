#pragma once
#include "Component.hpp"
#include "Observer.hpp"
#include <string>
#include <vector>
namespace dae {
class GameObject;

class PepperDisplay : public Component, public Observer {
public:
  PepperDisplay(GameObject &gameObject, std::string const &imagePath,
                int pepper);
  virtual void Notify(EventId event, GameObject *source) override;

private:
  void DecrementPepper();
  int m_Pepper{};
  GameObject *m_PepperText{nullptr};
  GameObject *m_PepperImage{nullptr};
};
} // namespace dae
