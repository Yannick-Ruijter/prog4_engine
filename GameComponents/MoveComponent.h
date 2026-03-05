#pragma once
#include <glm/glm.hpp>
#include "Component.h"
namespace dae
{
	class GameObject;
	class TransformComponent;
	class MoveComponent : public Component
	{
	public:
		MoveComponent(GameObject& owner);
		~MoveComponent() override = default;
		void Move(glm::vec3 const& displacement);
	private:
		TransformComponent* m_TransformComponent;
	};
}