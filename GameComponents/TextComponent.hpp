#pragma once
#include <string>
#include <memory>
#include "Component.hpp"
#include <SDL3/SDL.h>

namespace dae
{
	class Font;
	class Texture2DComponent;
	class TextComponent : public Component
	{
	public:
		void Update() override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);
		//std::shared_ptr<Texture2DComponent> GetTexture() const;

		TextComponent(GameObject& owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};
		Texture2DComponent* m_TextTexture{};
	};
}
