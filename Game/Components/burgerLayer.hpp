#pragma once
#include "Component.hpp"
namespace dae {
    class BurgerLayerPart;
    class LevelGrid;
    enum class BurgerLayerType { TopPaddy, Salad, Tomato, BottomPaddy };
    class BurgerLayer : public Component {
      public:
        BurgerLayer(
            GameObject &owner, BurgerLayerType layer, std::vector<GameObject *> const &players,
            LevelGrid *levelGrid);
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
        void CreateChildrenParts(std::vector<GameObject *> const &players, BurgerLayerType layer);
        std::vector<GameObject *> m_LayerParts;
        glm::vec2 m_LayerDimensions{};
        uint32_t m_NumberOfPartsSteppedOn{0};
        LevelGrid *m_LevelGrid;
        Transform *m_Transform{nullptr};
        Transform *m_NeighborBellowTransform{nullptr};
        BurgerLayer *m_NeighborBellowComponent{nullptr};
        float m_FallingSpeed{60.f};
    };
} // namespace dae
