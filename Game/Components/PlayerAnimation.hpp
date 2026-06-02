#pragma once
#include "Component.hpp"
#include <memory>
#include <string>
namespace dae {
    class PlayerAnimation : public Component {
      public:
        PlayerAnimation(GameObject &owner, std::string const &animationDataPath, std::string const &spriteSheetPath);
        ~PlayerAnimation();
        void Update();
        void SetAnimationState(std::string const &animState);

      private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
} // namespace dae
