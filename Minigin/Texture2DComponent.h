#pragma once
#include <glm/vec2.hpp>
#include <string>
#include "Component.h"

struct SDL_Texture;
namespace dae
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class GameObject;
	class Texture2DComponent: public Component
	{
	public:
		static void AddToGameObject(GameObject* parent, const std::string& fullPath);
		static void AddToGameObject(GameObject* parent, SDL_Texture* texture);
		static Texture2DComponent* GetFromObject(GameObject* object);

		SDL_Texture* GetSDLTexture() const;
		explicit Texture2DComponent(SDL_Texture* texture);
		explicit Texture2DComponent(const std::string& fullPath);
		~Texture2DComponent();

		glm::vec2 GetSize() const;

		Texture2DComponent(const Texture2DComponent &) = delete;
		Texture2DComponent(Texture2DComponent &&) = delete;
		Texture2DComponent & operator= (const Texture2DComponent &) = delete;
		Texture2DComponent & operator= (const Texture2DComponent &&) = delete;
	private:
		SDL_Texture* m_Texture;

		friend GameObject;
	};
}
