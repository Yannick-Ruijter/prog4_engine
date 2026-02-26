#pragma once
#include "Game.h"

class ImGui : public Game
{
public:
	ImGui() = default;
	~ImGui() override = default;
	void Initialize() override;
};