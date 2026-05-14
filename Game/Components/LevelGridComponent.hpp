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

      private:
        glm::ivec2 m_GridSize{};
        std::vector<std::vector<char>> m_Grid{};
        std::unordered_map<char, SDL_Texture *> m_TexturesLoaded;
    };
} // namespace dae
