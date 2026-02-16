#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class FpsComponent : public Component
	{
	public:
		FpsComponent(GameObject* parent);
		void Update(float deltaTime);
		float GetFps() const;
	private:
		float m_LastFps{};
		long long m_StartLoopTime{};
		long long m_EndLoopTime{};
	};
}