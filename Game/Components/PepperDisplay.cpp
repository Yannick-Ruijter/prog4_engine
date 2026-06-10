#include "GameObject.hpp"
#include "ObjectRenderer.hpp"
#include "PepperDisplay.hpp"
#include "ResourceManager.hpp"
#include "TextDisplay.hpp"
#include "Texture2DDisplay.hpp"
#include "sdbm_hash.hpp"
#include <memory>

dae::PepperDisplay::PepperDisplay(GameObject &gameObject,
                                  std::string const &imagePath, int pepper)
    : Component(gameObject), m_Pepper{pepper} {
  auto fontMain =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 36);
  auto lifeText = std::make_unique<GameObject>();
  lifeText->AddComponent<ObjectRenderer>();
  lifeText->AddComponent<TextDisplay>(std::to_string(pepper), fontMain);
  // parent owns it now
  lifeText->SetParent(GetOwner(), false);
  lifeText->GetComponent<Transform>()->SetLocalPosition(glm::vec2{40.f, 0.f});
  m_PepperText = lifeText.release();

  auto lifeImage = std::make_unique<GameObject>();
  lifeImage->AddComponent<ObjectRenderer>();
  lifeImage->AddComponent<Texture2DDisplay>(imagePath, 35, 40);
  // parent owns it now
  lifeImage->SetParent(GetOwner(), false);
  m_PepperImage = lifeImage.release();
}

void dae::PepperDisplay::DecrementPepper() {
  --m_Pepper;
  auto text = m_PepperText->GetComponent<TextDisplay>();
  text->SetText(std::to_string(m_Pepper));
}

void dae::PepperDisplay::Notify(EventId event, GameObject *) {
  if (event == "Attacked"_h) {
    DecrementPepper();
  }
}
