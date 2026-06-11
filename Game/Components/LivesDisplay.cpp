#include "GameObject.hpp"
#include "LivesDisplay.hpp"
#include "ObjectRenderer.hpp"
#include "ResourceManager.hpp"
#include "TextDisplay.hpp"
#include "Texture2DDisplay.hpp"
#include "sdbm_hash.hpp"
#include <memory>

dae::LivesDisplay::LivesDisplay(GameObject &gameObject,
                                std::string const &imagePath, int lives)
    : Component(gameObject), m_Lives{lives} {
  auto fontMain =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 36);
  auto lifeText = std::make_unique<GameObject>();
  lifeText->AddComponent<ObjectRenderer>();
  lifeText->AddComponent<TextDisplay>(std::to_string(lives), fontMain);
  // parent owns it now
  lifeText->SetParent(GetOwner(), false);
  lifeText->GetComponent<Transform>()->SetLocalPosition(glm::vec2{40.f, 0.f});
  m_LifeText = lifeText.release();

  auto lifeImage = std::make_unique<GameObject>();
  lifeImage->AddComponent<ObjectRenderer>();
  lifeImage->AddComponent<Texture2DDisplay>(imagePath, 35, 40);
  // parent owns it now
  lifeImage->SetParent(GetOwner(), false);
  m_LifeImage = lifeImage.release();
}

void dae::LivesDisplay::DecrementLives() {
  if (m_Lives == 0) {
    auto text = m_LifeText->GetComponent<TextDisplay>();
    text->SetText("/");
  } else {
    --m_Lives;
    auto text = m_LifeText->GetComponent<TextDisplay>();
    text->SetText(std::to_string(m_Lives));
  }
}

void dae::LivesDisplay::Notify(EventId event, GameObject *) {
  if (event == "OnEntityDeath"_h) {
    DecrementLives();
  }
}
