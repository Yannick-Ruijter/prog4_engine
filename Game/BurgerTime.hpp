#pragma once
#include "Game.hpp"
#include <memory>
namespace dae { 
	class Observer;
	class GameObject;
	class Achievements;
}
class BurgerTime : public Game
{
public:
	BurgerTime();
	~BurgerTime() override;
	void Initialize() override;
	void Destroy() override;
private:
	dae::GameObject* m_Player1{ nullptr };
	dae::GameObject* m_Player2{ nullptr };
	std::unique_ptr<dae::Achievements> m_Achievements{ nullptr };
	int m_StartingLives{ 3 };
};