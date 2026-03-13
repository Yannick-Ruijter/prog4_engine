#pragma once
#include <memory>
#include <string>
#include "Component.h"

namespace dae
{
	class GameObject;
	class TransformComponent;
	class Texture2DComponent;
	class RenderComponent: public Component 
	{
	public:
		RenderComponent(GameObject& owner);
		~RenderComponent() override = default;
		void Update() override;
		void Render() const override;

	private:
		TransformComponent* m_TransformComponent;
		Texture2DComponent* m_Texture2DComponent;
	};
}