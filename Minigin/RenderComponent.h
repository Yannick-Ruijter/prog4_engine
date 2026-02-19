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
		RenderComponent(GameObject* parent);
		~RenderComponent() = default;
		void Render() const;
	};
}