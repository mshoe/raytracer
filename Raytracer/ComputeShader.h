#pragma once
#ifndef SHADER_H2
#define SHADER_H2

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GL/glew.h>

class ComputeShader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	ComputeShader(const GLchar* computePath)
	{
		this->Program = LoadShader(computePath);
	}

	std::string readFile(const char *filePath) {
		std::string content;
		std::ifstream fileStream(filePath, std::ios::in);

		if (!fileStream.is_open()) {
			std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
			return "";
		}

		std::string line = "";
		while (!fileStream.eof()) {
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		return content;
	}

	GLuint LoadShader(const char *computePath) {
		GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);

		// Read shader
		std::string computeShaderStr = readFile(computePath);
		const char *computeShaderSrc = computeShaderStr.c_str();

		GLint result = GL_FALSE;
		int logLength;

		// Compile compute shader
		std::cout << "Compiling compute shader." << std::endl;
		glShaderSource(computeShader, 1, &computeShaderSrc, NULL);
		glCompileShader(computeShader);

		// Check compute shader
		glGetShaderiv(computeShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> computeShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(computeShader, logLength, NULL, &computeShaderError[0]);
		std::cout << &computeShaderError[0] << std::endl;

		std::cout << "Linking program" << std::endl;
		GLuint program = glCreateProgram();
		glAttachShader(program, computeShader);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programError((logLength > 1) ? logLength : 1);
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
		std::cout << &programError[0] << std::endl;

		glDeleteShader(computeShader);

		return program;
	}

	// Uses the current shader
	void Use()
	{
		glUseProgram(this->Program);
	}
};

#endif