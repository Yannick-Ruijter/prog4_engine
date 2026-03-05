#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class TextComponent;
	class FpsComponent : public Component
	{
	public:
		FpsComponent(GameObject& owner);
		~FpsComponent() override = default;
		void Update(float deltaTime) override;
		float GetFps() const;
	private:
		float m_LastFps{};
		TextComponent* m_TextComponent{ nullptr };
	};
}