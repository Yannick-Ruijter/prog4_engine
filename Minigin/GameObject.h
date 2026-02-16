#pragma once
#include <string>
#include <memory>
#include <vector>
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include <SDL3/SDL.h>
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "Component.h"

namespace dae
{
	class Font;
	class GameObject final 
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		void AddComponent(std::unique_ptr<Component> component);
		template <typename T>
		T* GetComponent() const
		{
			for (auto const& component : m_MyComponents)
			{
				T* castedComponent = dynamic_cast<T*>(component.get());
				if (castedComponent != nullptr) return castedComponent;
			}
			return nullptr;
		};

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::unique_ptr<Component>> m_MyComponents;
	};
}
