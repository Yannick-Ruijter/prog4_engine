#pragma once
#include <string>
#include <memory>

namespace dae
{
	class Font;
	class Texture2DComponent;
	class GameObject;
	class TransformComponent;
	class TextComponent final
	{
	public:
		static void AddToGameObject(GameObject* parent, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		static TextComponent* GetFromObject(GameObject* object);
		void Update();

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);
		std::shared_ptr<Texture2DComponent> GetTexture() const;

		TextComponent(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};
		std::shared_ptr<Texture2DComponent> m_textTexture{};

		friend GameObject;
	};
}
