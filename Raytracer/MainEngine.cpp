#include "MainEngine.h"

MainEngine::MainEngine()
{
	init = Init();
}

MainEngine::~MainEngine()
{
	CleanUp();
}

bool MainEngine::Init()
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	//image_width = 800;
	//image_height = 600;

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	glEnable(GL_MULTISAMPLE);

	// Create our window centered at 512x512 resolution
	mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		image_width, image_height, SDL_WINDOW_OPENGL);

	// Check that everything worked out okay
	if (!mainWindow)
	{
		std::cout << "Unable to create window\n";
		CheckSDLError(__LINE__);
		return false;
	}

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow);

	SetOpenGLAttributes();

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	// Apparently, this is needed for Apple
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *version = glGetString( GL_VERSION );
	printf( "Renderer: %s\n", renderer );
	printf( "OpenGL version %s\n", version );

	// Final assignments
	loop = true;

	Init_imgui();

	

	//inputEngine = new InputEngine;

	gameEngine = new GameEngine;
	gameEngine->image_width = image_width;
	gameEngine->image_height = image_height;

	//renderEngine = new VoxelRenderer(gameEngine->getVoxelEngine());

	//renderEngine->voxelEngine = gameEngine->getVoxelEngine();

	return true;
}

void MainEngine::CleanUp()
{

	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(mainContext);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}

void MainEngine::Render()
{
	// Render the visuals from the current game engine's render engine
	gameEngine->getRenderEngine()->Render();

	// imgui stuff
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSdlGL3_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			loop = false;
	}
	ImGui_ImplSdlGL3_NewFrame(mainWindow);
	
	menu();

	// Render the UI from the current game engine's render engine
	gameEngine->getRenderEngine()->RenderUI();

	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	ImGui::Render();

	SDL_GL_SwapWindow(mainWindow);
}

void MainEngine::Input()
{
	gameEngine->getInputEngine()->Input();
}

void MainEngine::Loop()
{
	deltaTime = 0;
	totalTime = 0;
	
	// imgui stuff
	bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

	while (loop)
	{
		
		time1 = std::chrono::high_resolution_clock::now();
		Input();
		loop = gameEngine->getInputEngine()->loop;
		Update();
		//renderEngine->UpdateAngle(inputEngine->angle);



		// Pause
		//if (!inputEngine->pause) {
			//std::cout << renderEngine->totalTime;
			//renderEngine->UpdateTime(deltaTime);
			Render();
		//}


		time2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(time2 - time1);
		deltaTime = time_span.count();
		totalTime += deltaTime;
	}
}

void MainEngine::Update()
{
	gameEngine->Update();
	gameEngine->deltaTime = deltaTime;
}

void MainEngine::Init_imgui()
{
	ImGui_ImplSdlGL3_Init(mainWindow);
}

void MainEngine::menu()
{
	//ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Menu");
	
	
	//if (ImGui::Button("Rotate+")) inputEngine->angle += 2.5f;
	//if (ImGui::Button("Rotate-")) inputEngine->angle -= 2.5f;
	if (ImGui::Button("Close")) loop = false;
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Text("by mshoe");
    ImGui::End();
}

bool MainEngine::SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

void MainEngine::PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}

void MainEngine::CheckSDLError(int line)
{
		std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SLD Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}
