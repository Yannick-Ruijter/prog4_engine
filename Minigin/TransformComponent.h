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
		void Update() override;
		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		void SetLocalPosition(float x, float y, float z = 0);
		void SetLocalPosition(const glm::vec3& position);
		void SetWorldPositionDirty() { m_UpdateWorldPos = true; }
	private:
		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		bool m_UpdateWorldPos{};
	};
}
