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
		RenderComponent(GameObject* owner);
		~RenderComponent() override = default;
		void Render() const override;
	};
}