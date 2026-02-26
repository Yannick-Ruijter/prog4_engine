#pragma once
#include "Game.h"

class TrashTheCache : public Game
{
public:
	TrashTheCache() = default;
	~TrashTheCache() override;
	void Initialize() override;
	void Render() override;
private:
	bool m_TrashCacheEx1ButtonPressed{ false };
	void ShowEx1Screen();
	void ShowEx2Screen() const;

	void CalculatePlotInfoEx1(int nrOfSamples);
};