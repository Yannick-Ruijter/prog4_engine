#pragma once
#include "Component.hpp"
namespace dae {
    class LayerPartComponent;
    enum class BurgerLayer { TopPaddy, Salad, Tomato, BottomPaddy };
    class BurgerLayerComponent : public Component {
      public:
        BurgerLayerComponent(GameObject &owner, BurgerLayer layer, std::vector<GameObject *> const &players);
        void OnLayerPartCollided();

      private:
        void CreateChildrenParts(std::vector<GameObject *> const &players, BurgerLayer layer);
        std::vector<LayerPartComponent *> m_LayerParts;
        int m_NumberOfPartsSteppedOn{0};
    };
} // namespace dae
