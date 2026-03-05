#include "MoveComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace dae;
MoveComponent::MoveComponent(GameObject& owner)
	:Component(owner)
{}