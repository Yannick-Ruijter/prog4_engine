#pragma once
#include "Component.hpp"
#include <memory>
#include <string>
namespace dae {
    class SpriteAnimation : public Component {
      public:
        SpriteAnimation(GameObject &owner, std::string const &animationDataPath, std::string const &spriteSheetPath);
        ~SpriteAnimation();
        void Update();
        void SetAnimationState(std::string const &animState);

      private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
} // namespace dae
