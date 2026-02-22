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
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& position);
	private:
		glm::vec3 m_Position;
	};
}
