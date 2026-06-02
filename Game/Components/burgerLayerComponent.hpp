#pragma once
#include "Component.hpp"
namespace dae {
    class LayerPartComponent;
    class LevelGridComponent;
    enum class BurgerLayer { TopPaddy, Salad, Tomato, BottomPaddy };
    class BurgerLayerComponent : public Component {
      public:
        BurgerLayerComponent(
            GameObject &owner, BurgerLayer layer, std::vector<GameObject *> const &players,
            LevelGridComponent *levelGrid);
        void OnLayerPartCollided();
        virtual void Update() override;
        void StartFalling();
        bool CanFall();
        bool IsFalling() const;
        static std::vector<GameObject *> AllBurgerLayers;

      private:
        void StopFalling();
        void CalculateLayerBellow();
        bool m_IsFalling{false};
        void CreateChildrenParts(std::vector<GameObject *> const &players, BurgerLayer layer);
        std::vector<GameObject *> m_LayerParts;
        glm::vec2 m_LayerDimensions{};
        uint32_t m_NumberOfPartsSteppedOn{0};
        LevelGridComponent *m_LevelGrid;
        TransformComponent *m_Transform{nullptr};
        TransformComponent *m_NeighborBellowTransform{nullptr};
        BurgerLayerComponent *m_NeighborBellowComponent{nullptr};
        float m_FallingSpeed{60.f};
    };
} // namespace dae
