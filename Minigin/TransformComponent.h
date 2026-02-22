#pragma once
#include <glm/glm.hpp>
#include "Component.h"
namespace dae
{
	class GameObject;
	class TransformComponent : public Component
	{
	public:
		TransformComponent(GameObject& owner, glm::vec3 const& position = {0, 0, 0});
		~TransformComponent() override = default;
		void Update(float deltaTime) override;
		const glm::vec3& GetWorldPosition() const { return m_WorldPosition; }
		void SetWorldPosition(float x, float y, float z = 0);
		void SetWorldPosition(const glm::vec3& position);
		void SetLocalPosition(const glm::vec3& position);
	private:
		glm::vec3 m_WorldPosition;
		glm::vec3 m_LastParentPosition;
		glm::vec3 m_LocalPosition;
	};
}
