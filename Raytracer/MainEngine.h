#pragma once
#include <iostream>
#include <string>
#include <chrono>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "engines.h"
#include "GameEngine.h"

class MainEngine
{
public:
	MainEngine();
	~MainEngine();

	// Management
	bool Init();
	bool init;
	void CleanUp();

	// Sub-engine functions
	void Render();
	void Input();

	// Looper and timer elements
	void Loop();
	void Update();
	bool loop;
	std::chrono::high_resolution_clock::time_point time1;
	std::chrono::high_resolution_clock::time_point time2;
	float deltaTime;
	float totalTime;

	// SDL
	// Our SDL_Window (just like with SDL2 wihout OpenGL)
	SDL_Window *mainWindow;
	int image_width =512;
	int image_height =512;

	// imgui
	void Init_imgui();

	void menu();

	// Our opengl context handle
	SDL_GLContext mainContext;
	std::string programName = "Dark World by mshoe";
	bool SetOpenGLAttributes();
	void PrintSDL_GL_Attributes();
	void CheckSDLError(int line);

	

private:

	// The real game logic specific to the game state is controlled here
	GameEngine *gameEngine = NULL;
};

enum Simulation {
	INFINITE_WELL_2D = 0,
	TEST = 1
};