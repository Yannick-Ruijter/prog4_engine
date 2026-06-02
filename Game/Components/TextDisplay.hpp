#pragma once
#include "Component.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>

namespace dae {
    class Font;
    class Texture2DDisplay;
    class TextDisplay : public Component {
      public:
        void Update() override;

        void SetText(const std::string &text);
        void SetColor(const SDL_Color &color);

        TextDisplay(
            GameObject &owner, const std::string &text, std::shared_ptr<Font> font,
            const SDL_Color &color = {255, 255, 255, 255});
        ~TextDisplay() override = default;
        TextDisplay(const TextDisplay &other) = delete;
        TextDisplay(TextDisplay &&other) = delete;
        TextDisplay &operator=(const TextDisplay &other) = delete;
        TextDisplay &operator=(TextDisplay &&other) = delete;

      private:
        bool m_needsUpdate{};
        std::string m_text{};
        SDL_Color m_color{255, 255, 255, 255};
        std::shared_ptr<Font> m_font{};
        Texture2DDisplay *m_TextTexture{};
    };
} // namespace dae
