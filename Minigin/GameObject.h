#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include <SDL3/SDL.h>

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent;
	class GameObject final 
	{
		Transform m_Transform;
		std::unique_ptr<RenderComponent> m_RenderComponent;
		std::shared_ptr<Texture2D> m_Texture;
		std::shared_ptr<TextComponent> m_TextComponent;
		std::unique_ptr<FpsComponent> m_FpsComponent;
	public:
		virtual void Update();
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		void AddRenderComponent();
		void AddFpsComponent();
		void AddTextComponent(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		void SetRenderText(std::string const& text);
		void SetRenderTextColor(const SDL_Color& color);
		float GetFps();

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};
}
