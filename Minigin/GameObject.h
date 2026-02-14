#pragma once
#include <string>
#include <memory>
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include <SDL3/SDL.h>
#include "TextComponent.h"
#include "Texture2DComponent.h"

namespace dae
{
	class Font;
	class GameObject final 
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		struct {
			std::unique_ptr<TransformComponent> transform;
			std::unique_ptr<RenderComponent> renderComponent;
			std::shared_ptr<Texture2DComponent> texture2DComponent;
			std::unique_ptr<TextComponent> textComponent;
			std::unique_ptr<FpsComponent> fpsComponent;
		} m_Components;

		friend TransformComponent;
		friend RenderComponent;
		friend Texture2DComponent;
		friend TextComponent;
		friend FpsComponent;
	};
}
