#include "Command.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MoveComponent.h"
#include "PlayerInput.h"
#include "ControllerInput.h"

using namespace dae;

GameObjectCommand::GameObjectCommand(GameObject& object)
	:m_GameObject{&object}
{
}

GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}

MoveObjectCommand::MoveObjectCommand(GameObject& object, MoveDirection direction)
	:GameObjectCommand(object)
	, m_MoveDirection{direction}
{
}
void MoveObjectCommand::Execute()
{
	if (m_MoveComponent == nullptr) m_MoveComponent = GetGameObject()->GetComponent<MoveComponent>();
	assert(m_MoveComponent != nullptr);
	m_MoveComponent->Move(m_MoveDirection);
}
