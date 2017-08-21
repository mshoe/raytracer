#include "RenderEngine.h"

RenderEngine::RenderEngine()
{
	Init();
}

RenderEngine::RenderEngine(GameEngine * gameEngine, int image_width, int image_height)
{
	this->gameEngine = gameEngine;
	this->image_width = image_width;
	this->image_height = image_height;
	Init();
}

RenderEngine::~RenderEngine()
{
	CleanUp();
}

bool RenderEngine::Init()
{
	std::cout << "Initializing render engine" << std::endl << std::endl;


	computeShader = new ComputeShader("ComputeShader.frag");
	shader = new Shader("SampleShader.vert", "SampleShader.frag");
	//camera = new Camera();

	BuildQuad();
	BuildTexture();

	
	// WORK GROUPS
	int work_grp_size[3], work_grp_inv;
	// maximum global work group (total work in a dispatch)
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_size[0] );
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_size[1] );
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_size[2] );
	printf( "max global (total) work group size x:%i y:%i z:%i\n", work_grp_size[0],
					work_grp_size[1], work_grp_size[2] );
	// maximum local work group (one shader's slice)
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0] );
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1] );
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2] );
	printf( "max local (in one shader) work group sizes x:%i y:%i z:%i\n",
					work_grp_size[0], work_grp_size[1], work_grp_size[2] );
	// maximum compute shader invocations (x * y * z)
	glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv );
	printf( "max computer shader invocations %i\n", work_grp_inv );


//	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
//printf("max local work group invocations %i\n", work_grp_inv);

	return true;
}

void RenderEngine::CleanUp()
{
	delete shader;
	delete computeShader;
}

void RenderEngine::BuildQuad()
{
	float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void RenderEngine::BuildTexture()
{
	glGenTextures(1, &tex_output);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_output);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image_width, image_height, 0, GL_RGBA, GL_FLOAT,
	 NULL);
	glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void RenderEngine::Render()
{
	// drawing loop


	  { // launch compute shaders!
		computeShader->Use();
		glDispatchCompute((GLuint)image_width, (GLuint)image_height, 1);
	  }
  
	  // make sure writing to image has finished before read
	  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  
	  { // normal drawing pass
		glClear(GL_COLOR_BUFFER_BIT);
		shader->Use();
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_output);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	  }

	
}

void RenderEngine::RenderUI()
{
	ImGui::Begin("Test GUI");


	if (ImGui::Button("Mine")) {}

    
	ImGui::End();

}
