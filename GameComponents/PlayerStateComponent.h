#pragma once
#include "Observer.h"
#include "Component.h"

namespace dae {
	class GameObject;
	enum class PlayerState {
		Idle,
		Running,
		Attacking,
		Climbing,
		Hit,
		Count
	};
	class PlayerStateComponent : public Component, public Observer {
	public:
		PlayerStateComponent(GameObject& owner);
		void Notify(unsigned int eventId, GameObject* source);
		PlayerState GetState() const;
	private:
		PlayerState m_CurrentPlayerState{ PlayerState::Idle };
	};
}