#pragma once
#include <iostream>
#include <SDL2/SDL.h>

#include "engines.h"
#include "RenderEngine.h"
#include "InputEngine.h"

class GameEngine {
public:
	GameEngine();
	GameEngine(int image_width, int image_height);
	~GameEngine();

	bool Init();
	void CleanUp();

	

	//  Game engine logic goes here
	void Update();

	// Game engine "engine" tasks go here
	void Loop();

	RenderEngine *getRenderEngine();
	InputEngine *getInputEngine();

	int image_width;
	int image_height;

	float deltaTime = 0;

private:
	RenderEngine *renderEngine = NULL;
	InputEngine *inputEngine = NULL;
};