#pragma once
#include <SDL3/SDL.h>
#include "Singleton.h"
#include "Game.h"

namespace dae
{
	class Texture2DComponent;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
		Game* m_Game{};
	public:
		void Init(SDL_Window* window, Game* game);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2DComponent& texture, float x, float y) const;
		void RenderTexture(const Texture2DComponent& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;
		SDL_Window* GetWindow() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

