#include "Button.hpp"
#include "ControllerInput.hpp"
#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "Health.hpp"
#include "InputManager.hpp"
#include "PlayerInput.hpp"
#include "ServiceProvider.hpp"
#include "SoundSystem.hpp"
#include "Subject.hpp"
#include "TimeManager.hpp"
#include "Transform.hpp"
#include "sdbm_hash.hpp"
#include <GameManager.hpp>
#include <GameSceneLoader.hpp>
#include <LoadSceneLoader.hpp>
#include <SceneManager.hpp>
using namespace dae;

GameObjectCommand::GameObjectCommand(GameObject &object)
    : m_GameObject{&object} {}

GameObject *GameObjectCommand::GetGameObject() const { return m_GameObject; }

MoveObjectCommand::MoveObjectCommand(GameObject &object, Direction direction,
                                     float speed)
    : GameObjectCommand(object), m_Transform{object.GetComponent<Transform>()},
      m_TimeManager{&TimeManager::GetInstance()}, m_Speed{speed} {
  if (direction == Direction::Up)
    m_MoveDir = glm::vec2{0.f, -1.f};
  if (direction == Direction::Down)
    m_MoveDir = glm::vec2{0.f, 1.f};
  if (direction == Direction::Left)
    m_MoveDir = glm::vec2{-1.f, 0.f};
  if (direction == Direction::Right)
    m_MoveDir = glm::vec2{1.f, 0.f};
}
void MoveObjectCommand::Execute() {
  glm::vec2 displacement{m_MoveDir * m_TimeManager->GetDeltaTime() * m_Speed};
  m_Transform->SetLocalPosition(m_Transform->GetLocalPosition() + displacement);
}

dae::DamagePlayer::DamagePlayer(GameObject &object, GameObject &target)
    : GameObjectCommand(object),
      m_TargetHealthComponent(target.GetComponent<Health>()) {
  assert(m_TargetHealthComponent != nullptr &&
         "target player for damage command needs a health component");
}

void dae::DamagePlayer::Execute() {
  m_TargetHealthComponent->Damage();
  ServiceProvider::GetSoundSystem().Play(0, 0.5f);
}

dae::PickUpItemCommand::PickUpItemCommand(GameObject &object)
    : GameObjectCommand(object),
      m_PlayerPickedUpItemEvent{std::make_unique<Subject>()} {}

void dae::PickUpItemCommand::Execute() {
  // does nothing else yet since there is no picking up in my game yet and it's
  // currently just increment score will have functionality here in the future
  m_PlayerPickedUpItemEvent->NotifyObservers("ItemPickedUp"_h, GetGameObject());
  ServiceProvider::GetSoundSystem().Play(1, 0.5f);
}

Subject *dae::PickUpItemCommand::GetSubject() const {
  return m_PlayerPickedUpItemEvent.get();
}

Button *ButtonCommand::CurrentButton = nullptr;
dae::NavigateButtonCommand::NavigateButtonCommand(Direction dir)
    : m_NavigateDir{dir} {}

void dae::ButtonCommand::SetInitialButton(Button *button) {
  CurrentButton = button;
}

void dae::NavigateButtonCommand::Execute() {
  CurrentButton->LoseFocus();
  CurrentButton = CurrentButton->GetNeighbor(m_NavigateDir);
  CurrentButton->GainFocus();
}

void dae::PressButtonCommand::Execute() { CurrentButton->Execute(); }

void dae::ToggleSoundCommand::Execute() {
  ServiceProvider::GetSoundSystem().ToggleVolume();
}

dae::AdvanceSceneCommand::AdvanceSceneCommand(GameManager *manager)
    : m_Manager{manager} {}

void dae::AdvanceSceneCommand::Execute() {
  auto levelInfo{m_Manager->GetLevelInfo()};
  levelInfo.level++;
  if (levelInfo.level >= 3)
    levelInfo.level = 0;
  levelInfo.burgerInfos.clear();
  SceneManager::GetInstance().LoadScene<LoadSceneLoader>(levelInfo);
}
