#pragma once
#include "Component.hpp"
#include <memory>
#include <string>
namespace dae
{
    class PlayerAnimationComponent : public Component
    {
      public:
        PlayerAnimationComponent(
            GameObject &owner, std::string const &animationDataPath, std::string const &spriteSheetPath);
        ~PlayerAnimationComponent();
        void Update();
        void SetAnimationState(std::string const &animState);

      private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
} // namespace dae
