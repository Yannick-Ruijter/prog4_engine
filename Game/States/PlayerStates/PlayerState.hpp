#pragma once
#include "Entity.hpp"
#include "Observer.hpp"
#include <memory>
namespace dae {
    class Entity;
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
        PlayerState(Entity &player);
        Entity *m_Player;
    };
} // namespace dae
