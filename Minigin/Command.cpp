#include "Command.h"
#include "GameObject.h"
#include "InputManager.h"
#include "PlayerInput.h"
#include "ControllerInput.h"
#include "TransformComponent.h"
#include "TimeManager.h"

using namespace dae;

GameObjectCommand::GameObjectCommand(GameObject& object)
	:m_GameObject{&object}
{
}

GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}

MoveObjectCommand::MoveObjectCommand(GameObject& object, MoveDirection direction, float speed)
	:GameObjectCommand(object)
	, m_TransformComponent{object.GetComponent<TransformComponent>()}
	, m_TimeManager{&TimeManager::GetInstance()}
	, m_Speed{speed}
{
	if(direction == MoveDirection::Up) m_MoveDir = glm::vec3{ 0.f, -1.f, 0.f };
	if(direction == MoveDirection::Down) m_MoveDir = glm::vec3{ 0.f, 1.f, 0.f };
	if(direction == MoveDirection::Left) m_MoveDir = glm::vec3{ -1.f, 0.f, 0.f };
	if(direction == MoveDirection::Right) m_MoveDir = glm::vec3{ 1.f, 0.f, 0.f };
}
void MoveObjectCommand::Execute()
{
	glm::vec3 displacement{ m_MoveDir * m_TimeManager->GetDeltaTime() * m_Speed };
	m_TransformComponent->SetLocalPosition(m_TransformComponent->GetLocalPosition() + displacement);
}
