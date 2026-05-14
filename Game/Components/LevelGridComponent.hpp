#include "Component.hpp"
#include "Texture2DComponent.hpp"
#include "glm/glm.hpp"
#include <SDL3/SDL.h>
#include <map>
#include <string>
namespace dae
{
    class GameObject;
    class LevelGridComponent : public Component
    {
      public:
        LevelGridComponent(
            GameObject &owner, glm::ivec2 const &gridSize, std::string const &levelFile,
            std::unordered_map<char, std::string> textures);

        virtual void Render() const override;
        bool IsOnPlatform(glm::ivec2 const &topleft, glm::ivec2 size);
        bool IsOnLadder(glm::ivec2 const &topleft, glm::ivec2 size);

      private:
        glm::ivec2 m_GridSize{};
        std::vector<std::vector<char>> m_Grid{};
        std::unordered_map<char, SDL_Texture *> m_TexturesLoaded;

        char GetTile(glm::ivec2 const &pos);
    };
} // namespace dae
