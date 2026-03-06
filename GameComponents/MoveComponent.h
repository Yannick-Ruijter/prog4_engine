#pragma once
#include <glm/glm.hpp>
#include "Component.h"
namespace dae
{
	class GameObject;
	class TransformComponent;
	enum class MoveDirection
	{
		Up,
		Down,
		Left,
		Right,

	};

	class MoveComponent : public Component
	{
	public:
		MoveComponent(GameObject& owner, float speed = 100.f);
		~MoveComponent() override = default;
		void Update(float deltaTime) override;
		void Move(MoveDirection direction);
	private:
		TransformComponent* m_TransformComponent;
		float m_Speed;
		glm::vec3 m_DisplacementBuffer{};
		bool m_ShouldMove{ false };
	};
}