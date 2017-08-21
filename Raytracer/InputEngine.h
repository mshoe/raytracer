#pragma once
#include <SDL2/SDL.h>
#include <iostream>

#include "engines.h"
#include "GameEngine.h"

class InputEngine
{
public:
	InputEngine();
	InputEngine(GameEngine *gameEngine) {this->gameEngine = gameEngine; Init(); }
	~InputEngine();

	bool Init();
	bool init;
	void CleanUp();

	void Input();

	bool pause = false;
	bool loop = true;

	float angle = 0.f;

	GameEngine *gameEngine = NULL;
private:

};