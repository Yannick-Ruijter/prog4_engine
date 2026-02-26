#pragma once
#include "Game.h"

class TrashTheCache : public Game
{
public:
	TrashTheCache() = default;
	~TrashTheCache() override = default;
	void Initialize() override;
	void Render() const override;
};