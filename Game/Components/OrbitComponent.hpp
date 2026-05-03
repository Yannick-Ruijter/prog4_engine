#pragma once
#include "Component.hpp"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class TimeManager;
	class OrbitComponent : public Component
	{
	public:
		OrbitComponent(GameObject& owner, float orbitOffset, float orbitSpeed);
		void Update() override;
	private:
		float m_OrbitOffset{};
		float m_OrbitSpeed{};
		float m_CurrentAngle{};
		TimeManager* m_TimeManager{nullptr};
	}; 
}