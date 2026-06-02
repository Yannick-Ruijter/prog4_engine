#pragma once
#include "SceneManager.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"

class Game {
public:
	Game();
	virtual ~Game() = default;
	virtual void Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();
};