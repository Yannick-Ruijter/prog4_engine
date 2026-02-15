#pragma once
#include <memory>
#include <string>
#include "Component.h"

namespace dae
{
	class Texture2DComponent;
	class GameObject;
	class TransformComponent;
	class RenderComponent: public Component 
	{
	public:
		static void AddToGameObject(GameObject* parent);
		static RenderComponent* GetFromObject(GameObject* object);

		RenderComponent() = default;
		~RenderComponent() = default;
		void Render(Texture2DComponent* texture, TransformComponent const* transform) const;

	private:

		friend GameObject;
	};
}