#pragma once
#include "Observer.hpp"
#include "PlayerController.hpp"
#include <memory>
namespace dae {
    class PlayerController;
    class PlayerState {
      public:
        PlayerState(PlayerState const &other) = default;
        PlayerState(PlayerState &&other) = default;
        PlayerState &operator=(PlayerState const &other) = default;
        PlayerState &operator=(PlayerState &&other) = default;

        virtual ~PlayerState();
        virtual std::unique_ptr<PlayerState> HandleInput();
        virtual void Update();
        virtual void OnEnter();
        virtual void OnExit();

      protected:
        PlayerState(PlayerController &player);
        PlayerController *m_Player;
    };
} // namespace dae
