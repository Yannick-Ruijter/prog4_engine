#include "Component.hpp"
#include "glm/glm.hpp"
namespace dae
{
    class GameObject;
    class LevelGridComponent : public Component
    {
      public:
        LevelGridComponent(GameObject &owner, glm::ivec2 const &gridDimensions, glm::ivec2 const &windowDimensions);

        virtual void Render() const override;

      private:
        glm::ivec2 m_GridNumbers{};
        glm::vec2 m_GridDimensions{};
    };
} // namespace dae
