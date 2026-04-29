#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "PlayerInput.hpp"
#include "ControllerInput.hpp"
#include "TransformComponent.hpp"
#include "TimeManager.hpp"
#include "HealthComponent.hpp"
#include "Subject.hpp"
#include "sdbm_hash.hpp"
#include "PlayerStateComponent.hpp"
#include "SDL_SoundSystem.hpp"
using namespace dae;

GameObjectCommand::GameObjectCommand(GameObject& object)
	:m_GameObject{ &object }
{
}

GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}

MoveObjectCommand::MoveObjectCommand(GameObject& object, MoveDirection direction, float speed)
	:GameObjectCommand(object)
	, m_TransformComponent{ object.GetComponent<TransformComponent>() }
	, m_TimeManager{ &TimeManager::GetInstance() }
	, m_Speed{ speed }
{
	if (direction == MoveDirection::Up) m_MoveDir = glm::vec3{ 0.f, -1.f, 0.f };
	if (direction == MoveDirection::Down) m_MoveDir = glm::vec3{ 0.f, 1.f, 0.f };
	if (direction == MoveDirection::Left) m_MoveDir = glm::vec3{ -1.f, 0.f, 0.f };
	if (direction == MoveDirection::Right) m_MoveDir = glm::vec3{ 1.f, 0.f, 0.f };
}
void MoveObjectCommand::Execute()
{
	glm::vec3 displacement{ m_MoveDir * m_TimeManager->GetDeltaTime() * m_Speed };
	m_TransformComponent->SetLocalPosition(m_TransformComponent->GetLocalPosition() + displacement);
}

dae::DamagePlayer::DamagePlayer(GameObject& object, GameObject& target)
	:GameObjectCommand(object)
	, m_TargetHealthComponent(target.GetComponent<HealthComponent>())
	, m_Ss{ ServiceProvider::GetInstance().GetService<SDL_SoundSystem>() }
{
	assert(m_TargetHealthComponent != nullptr && "target player for damage command needs a health component");
}

void dae::DamagePlayer::Execute()
{
	m_TargetHealthComponent->Damage();
	m_Ss->Play(0, 0.5f);
}

dae::PickUpItemCommand::PickUpItemCommand(GameObject& object)
	:GameObjectCommand(object)
	, m_PlayerPickedUpItemEvent{ std::make_unique<Subject>() }
	, m_Ss{ServiceProvider::GetInstance().GetService<SDL_SoundSystem>()}
{
}

void dae::PickUpItemCommand::Execute()
{
	//does nothing else yet since there is no picking up in my game yet and it's currently just increment score
	//will have functionality here in the future
	m_PlayerPickedUpItemEvent->NotifyObservers("ItemPickedUp"_h, GetGameObject());
	m_Ss->Play(1, 0.5f);
}

Subject* dae::PickUpItemCommand::GetSubject() const
{
	return m_PlayerPickedUpItemEvent.get();
}

dae::MovePlayerCommand::MovePlayerCommand(GameObject& object, MoveDirection direction, PlayerStateComponent* state, float speed)
	:MoveObjectCommand(object, direction, speed), m_PlayerState{ state }
	, m_OnPlayerStartedMove{ std::make_unique<Subject>() }
{
}

void dae::MovePlayerCommand::Execute()
{
	switch (m_PlayerState->GetState())
	{
	case PlayerState::Idle:
		[[fallthrough]];
	case PlayerState::Running:
		m_OnPlayerStartedMove->NotifyObservers("OnPlayerStartedMoving"_h, GetGameObject());
		MoveObjectCommand::Execute();
		break;
	default:
		return;
	}
}

void dae::MovePlayerCommand::StopExecution()
{
	if (m_PlayerState->GetState() == PlayerState::Running)
		m_OnPlayerStartedMove->NotifyObservers("OnPlayerStoppedMoving"_h, GetGameObject());
}

Subject* dae::MovePlayerCommand::GetSubject() const
{
	return m_OnPlayerStartedMove.get();
}
