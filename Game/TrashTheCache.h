#pragma once
#include "Game.h"

class TrashTheCache : public Game
{
public:
	TrashTheCache() = default;
	~TrashTheCache() override;
	void Initialize() override;
	void Render() const override;
private:
	void ShowEx1Screen() const;
	void ShowEx2Screen() const;
};