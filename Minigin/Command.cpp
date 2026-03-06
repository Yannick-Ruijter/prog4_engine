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

MoveObjectCommand::MoveObjectCommand(GameObject& object, PlayerInput& input, uint32_t up, uint32_t down, uint32_t left, uint32_t right)
	:GameObjectCommand(object)
	,m_MoveComponent{object.GetComponent<MoveComponent>()}
	,m_PlayerInput{&input}
	, m_InputButtons{ up, down, left, right }
{
}
void MoveObjectCommand::Execute()
{
	if (m_MoveComponent == nullptr) m_MoveComponent = GetGameObject()->GetComponent<MoveComponent>();
	assert(m_MoveComponent != nullptr);
	auto controllerInput = dae::InputManager::GetInstance().GetControllerInput();
	if (m_PlayerInput->IsButtonPressed(m_InputButtons[0])) m_MoveComponent->Move(MoveDirection::Up);
	if (m_PlayerInput->IsButtonPressed(m_InputButtons[1])) m_MoveComponent->Move(MoveDirection::Down);
	if (m_PlayerInput->IsButtonPressed(m_InputButtons[2])) m_MoveComponent->Move(MoveDirection::Left);
	if (controllerInput->IsButtonPressed(m_InputButtons[3])) m_MoveComponent->Move(MoveDirection::Right);
}
