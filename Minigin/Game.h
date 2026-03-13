#pragma once
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

class Game {
public:
	Game();
	virtual ~Game() = default;
	virtual void Initialize();
	virtual void Update();
	virtual void Render();
};