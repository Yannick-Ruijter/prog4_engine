#include "Command.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MoveComponent.h"

#define GAMEPAD_DPAD_UP 1
#define GAMEPAD_DPAD_DOWN 2
#define GAMEPAD_DPAD_LEFT 4
#define GAMEPAD_DPAD_RIGHT 8
using namespace dae;

GameObjectCommand::GameObjectCommand(GameObject& object)
	:m_GameObject{&object}
{
}

GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}

MoveObjectCommand::MoveObjectCommand(GameObject& object)
	:GameObjectCommand(object)
	,m_MoveComponent{object.GetComponent<MoveComponent>()}
{
}

void MoveObjectCommand::Execute()
{
	if (m_MoveComponent == nullptr) m_MoveComponent = GetGameObject()->GetComponent<MoveComponent>();
	assert(m_MoveComponent != nullptr);
	auto controllerInput = dae::InputManager::GetInstance().GetControllerInput();
	if (controllerInput->IsButtonPressed(GAMEPAD_DPAD_UP)) m_MoveComponent->Move(MoveDirection::Up);
	if (controllerInput->IsButtonPressed(GAMEPAD_DPAD_DOWN)) m_MoveComponent->Move(MoveDirection::Down);
	if (controllerInput->IsButtonPressed(GAMEPAD_DPAD_LEFT)) m_MoveComponent->Move(MoveDirection::Left);
	if (controllerInput->IsButtonPressed(GAMEPAD_DPAD_RIGHT)) m_MoveComponent->Move(MoveDirection::Right);
}
