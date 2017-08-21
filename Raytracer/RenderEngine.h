#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "engines.h"
#include "GameEngine.h"
#include "ComputeShader.h"
#include "Shader.h"

class RenderEngine {
public:
	RenderEngine();
	RenderEngine(GameEngine *gameEngine, int image_width, int image_height);
	~RenderEngine();

	// Initialization function
	// Sets up shaders and rendering environment
	bool Init();
	
	// Clean up function
	// Gets rid of all dynamic memory
	void CleanUp();

	void BuildQuad();
	void BuildTexture();

	// Program data from MainEngine
	float time = 0;
	float angle = 0;

	//virtual void UpdateDimensions(int image_width, int image_height) {};
	void UpdateTime(float time) {};
	void SetTime(float time) {};
	void UpdateAngle(float angle) { this->angle = angle; };


	void Render();
	void RenderUI();

	// dimensions
	int image_width;
	int image_height;
	

	// Just a pointer
	GameEngine *gameEngine = NULL;

protected:
	GLuint VBO, VAO, EBO;
	GLuint tex_output;
	ComputeShader *computeShader = NULL;
	Shader *shader = NULL;
};