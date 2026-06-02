#pragma once
#include "Component.hpp"
#include <glm/vec2.hpp>
#include <string>

struct SDL_Texture;
namespace dae {
    /**
     * Simple RAII wrapper for an SDL_Texture
     */
    class GameObject;
    class Texture2DDisplay : public Component {
      public:
        SDL_Texture *GetSDLTexture() const;
        void SetSDLTexture(SDL_Texture *texture);
        explicit Texture2DDisplay(GameObject &owner, SDL_Texture *texture);
        explicit Texture2DDisplay(GameObject &owner, const std::string &fullPath, int width = 0, int height = 0);
        ~Texture2DDisplay() override;

        glm::vec2 GetSize() const;

        Texture2DDisplay(const Texture2DDisplay &) = delete;
        Texture2DDisplay(Texture2DDisplay &&) = delete;
        Texture2DDisplay &operator=(const Texture2DDisplay &) = delete;
        Texture2DDisplay &operator=(const Texture2DDisplay &&) = delete;

      private:
        SDL_Texture *m_Texture;
    };
} // namespace dae
