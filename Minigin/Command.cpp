#include "Command.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MoveComponent.h"
GameObjectCommand::GameObjectCommand(GameObject& object)
	:m_GameObject{&object}
{
}

GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}

void MoveObjectCommand::Execute()
{

}
