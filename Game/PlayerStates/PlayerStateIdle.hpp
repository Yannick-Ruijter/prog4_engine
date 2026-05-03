#include "PlayerState.hpp"

namespace dae
{
	class PlayerStateIdle : public PlayerState {
	public:
		PlayerStateIdle(PlayerComponent& player);
		~PlayerStateIdle();
		std::unique_ptr<PlayerState> HandleInput();
		void Update();
		void OnEnter();
		void OnExit();
		void Notify(unsigned int eventId, GameObject* source);

	private:
		bool m_StartedMoving{ false };
	};
}