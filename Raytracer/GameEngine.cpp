#include "GameEngine.h"

GameEngine::GameEngine()
{
	Init();
}

GameEngine::~GameEngine()
{
	CleanUp();
}

bool GameEngine::Init()
{

	// Voxel Renderer needs direct access to Voxel Engine
	renderEngine = new RenderEngine(this, image_width, image_height);
	inputEngine = new InputEngine(this);

	SDL_CaptureMouse(SDL_TRUE);
	return true;
}

void GameEngine::CleanUp()
{
	if (renderEngine != NULL)
		delete renderEngine;
	if (inputEngine != NULL)
		delete inputEngine;
}

void GameEngine::Update()
{
}

void GameEngine::Loop()
{
}

RenderEngine * GameEngine::getRenderEngine()
{
	return renderEngine;
}

InputEngine * GameEngine::getInputEngine()
{
	return inputEngine;
}