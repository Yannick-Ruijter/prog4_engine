#pragma once

#include "Component.hpp"
#include "Observer.hpp"
#include <memory>
namespace dae {
    class PlayerAnimation;
    class PlayerState;
    class InputProvider;
    class LevelGrid;
    class PlayerController final : public Component {
      public:
        PlayerController(GameObject &owner, std::unique_ptr<InputProvider> input, LevelGrid *level);
        ~PlayerController();
        void Update();
        GameObject *GetPlayer();
        PlayerAnimation *GetPlayerAnimation() const;
        InputProvider *GetInput() const;
        LevelGrid *GetLevel() const;
        float GetMoveSpeed() const;

      private:
        PlayerAnimation *m_PlayerAnimation;
        std::unique_ptr<PlayerState> m_CurrentState;
        std::unique_ptr<InputProvider> m_Input;
        LevelGrid *m_Level;
        float m_MoveSpeed{100.f};
    };
} // namespace dae
