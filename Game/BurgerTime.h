#pragma once
#include "Game.h"
#include <memory>
namespace dae { 
	class Observer;
	class GameObject;
}
class BurgerTime : public Game
{
public:
	BurgerTime();
	~BurgerTime() override;
	void Initialize() override;

private:
	std::unique_ptr < dae::Observer> m_Player1HealthObserver{ nullptr };
	dae::GameObject* m_Player1{ nullptr };
	dae::GameObject* m_Player2{ nullptr };
};