#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class OrbitComponent : public Component
	{
	public:
		OrbitComponent(GameObject& owner, float orbitRadius, float orbitSpeed);
		void Update(float deltaTime) override;
	private:
		glm::vec3 m_Center{};
		float m_OrbitRadius{};
		float m_OrbitSpeed{};
		float m_CurrentAngle{};
	}; 
}