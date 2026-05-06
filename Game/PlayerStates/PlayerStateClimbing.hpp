#include "PlayerState.hpp"

namespace dae
{
    class PlayerStateClimbing : public PlayerState
    {
      public:
        PlayerStateClimbing(PlayerComponent &player);
        ~PlayerStateClimbing();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();
        void Notify(unsigned int eventId, GameObject *source);

      private:
        bool m_PlayerStoppedClimbing{false};
    };
} // namespace dae
