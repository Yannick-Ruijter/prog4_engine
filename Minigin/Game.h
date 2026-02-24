#pragma once
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

class Game {
public:
	Game();
	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void Render() const;
};