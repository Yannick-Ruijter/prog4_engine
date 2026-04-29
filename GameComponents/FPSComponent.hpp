#pragma once
#include "Component.hpp"

namespace dae
{
	class TimeManager;
	class GameObject;
	class TextComponent;
	class FpsComponent : public Component
	{
	public:
		FpsComponent(GameObject& owner);
		~FpsComponent() override = default;
		void Update() override;
		float GetDeltaTime() const;
	private:
		float m_LastFps{};
		TextComponent* m_TextComponent{ nullptr };
		TimeManager* m_TimeManager{ nullptr };
	};
}