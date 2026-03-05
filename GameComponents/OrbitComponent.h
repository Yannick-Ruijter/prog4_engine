#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class OrbitComponent : public Component
	{
	public:
		OrbitComponent(GameObject& owner, float orbitOffset, float orbitSpeed);
		void Update(float deltaTime) override;
	private:
		float m_OrbitOffset{};
		float m_OrbitSpeed{};
		float m_CurrentAngle{};
	}; 
}