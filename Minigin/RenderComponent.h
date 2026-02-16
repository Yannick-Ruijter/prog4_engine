#pragma once
#include <memory>
#include <string>
#include "Component.h"

namespace dae
{
	class GameObject;
	class RenderComponent: public Component 
	{
	public:
		static void AddToGameObject(GameObject* parent);
		static RenderComponent* GetFromObject(GameObject* object);

		RenderComponent(GameObject* parent);
		~RenderComponent() = default;
		void Render() const;
	};
}