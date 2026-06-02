#include "Component.hpp"
#include "Texture2DDisplay.hpp"
#include "glm/glm.hpp"
#include <SDL3/SDL.h>
#include <map>
#include <string>
namespace dae {
    class GameObject;
    class LevelGrid : public Component {
      public:
        LevelGrid(
            GameObject &owner, glm::ivec2 const &gridSize, std::string const &levelFile,
            std::unordered_map<char, std::string> textures);

        virtual void Render() const override;
        bool IsOnPlatform(glm::vec2 const &topleft, glm::vec2 size);
        bool IsOnLadder(glm::vec2 const &topleft, glm::vec2 size);
        bool IsOnBurgerBowl(glm::vec2 const &topleft, glm::vec2 size);
        float RoundToPlatformHeight(float yPos);
        int GetGridSize() const;

      private:
        glm::ivec2 m_GridSize{};
        std::vector<std::vector<char>> m_Grid{};
        std::unordered_map<char, SDL_Texture *> m_TexturesLoaded;

        char GetTile(glm::vec2 const &pos);
    };
} // namespace dae
