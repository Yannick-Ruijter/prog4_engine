#pragma once
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

class Game {
public:
	Game();
	void Initialize();
	void Update(float deltaTime);
	void Render() const;
};