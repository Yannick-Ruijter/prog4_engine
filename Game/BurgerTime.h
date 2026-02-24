#pragma once
#include "Game.h"

class BurgerTime : public Game
{
public:
	BurgerTime() = default;
	~BurgerTime() override = default;
	void Initialize() override;
};