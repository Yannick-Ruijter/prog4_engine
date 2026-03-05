#pragma once
#include "Game.h"
#include "imgui_plot.h"

class TrashTheCache : public Game
{
public:
	TrashTheCache() = default;
	~TrashTheCache() override;
	void Initialize() override;
	void Render() override;
};