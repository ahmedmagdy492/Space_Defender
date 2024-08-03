#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "../include/common.h"

class ShaderProgram {
private:
	unsigned int programId;

	unsigned int CreateShader(std::string& shaderSrc, unsigned int shaderType) {
		int success;
		char infoLog[512];

		unsigned int shaderId = glCreateShader(shaderType);
		const char* src = shaderSrc.c_str();
		glShaderSource(shaderId, 1, &src, NULL);
		glCompileShader(shaderId);

		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
			throw std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
		}

		return shaderId;
	}

public:
	ShaderProgram(std::string& vertexSrcCode, std::string& fragmentSrcCode) {
		unsigned int vertexShader = CreateShader(vertexSrcCode, (unsigned int)GL_VERTEX_SHADER);
		unsigned int fragmentShader = CreateShader(fragmentSrcCode, (unsigned int)GL_FRAGMENT_SHADER);

		programId = glCreateProgram();
		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);

		glLinkProgram(programId);

		int success;
		char infoLog[512];

		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
				infoLog << std::endl;
			throw std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED");
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Use() {
		glUseProgram(programId);
	}
};

class Shape {
protected:
	unsigned int vboId;
	unsigned int eboId;

public:
	Vector3 position;
	Color color;
	ShaderProgram* shaderProgram;

	Shape();

	~Shape();

	virtual void Init() = 0;
	virtual void Draw() = 0;
};

class Rectangle : public Shape {
private:
	float ConvertToNDCForX(float x);
	float ConvertToNDCForY(float y);

public:
	float width, height;

	Rectangle(Vector3 position, float width, float height, Color color);

	void Init();

	void Draw();
};

class Scene {
public:
	virtual void Init() = 0;

	virtual void ProcessInput(GLFWwindow* window) = 0;

	virtual void Render() = 0;
};

class MenuScene : public Scene {
private:
	std::vector<Shape*> shapes;

public:
	void Init();

	void ProcessInput(GLFWwindow* window);

	void Render();

	~MenuScene();
};