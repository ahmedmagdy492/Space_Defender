#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "../include/common.h"
#include "../include/stb_image/stb_image.h"

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

	unsigned int GetProgramId() {
		return programId;
	}

	void Use() {
		glUseProgram(programId);
	}

	void SetBool(const std::string& name, bool value) const {
		int location = glGetUniformLocation(programId, name.c_str());
		glUniform1i(location, (int)value);
	}

	void SetInt(const std::string& name, int value) const {
		int location = glGetUniformLocation(programId, name.c_str());
		glUniform1i(location, value);
	}

	void SetFloat(const std::string& name, float value) const {
		int location = glGetUniformLocation(programId, name.c_str());
		glUniform1f(location, value);
	}

	void SetVector3(const std::string& name, Vector3 vec) {
		int location = glGetUniformLocation(programId, name.c_str());
		float values[] = { vec.x, vec.y, vec.z };
		glUniform3fv(location, 1, values);
	}
};

class Shape {
protected:
	unsigned int vboId;
	unsigned int eboId;
	unsigned int vaoId;

	float ConvertToNDCForX(float x);
	float ConvertToNDCForY(float y);

public:
	Vector3 position;
	Color color;

	virtual void Init() = 0;
	virtual void Draw() = 0;
};

class Rectangle : public Shape {
public:
	int width, height;

	Rectangle(Vector3 position, int width, int height, Color color);

	void Init();

	void Draw();
};

class Texture2D : public Shape {
private:
	unsigned int textureId;

	void SetupTextureBuffers(Vector3 newPosition);
public:
	int width, height;
	int imgWidth, imgHeight;
	std::string imagePath;
	int textureUnit;

	Texture2D(Vector3 position, int width, int height, std::string imagePath, unsigned int textureUnit);

	void UpdateTexture(Vector3 position);

	void Init();

	void Draw();

	void SetOption(GLenum name, int value);
};


// Game Logic Models

class Player {
public:
	Texture2D* texture;
	float health;

	Player(Vector3 position);

	void Move(Vector3 velocity);

	void Render();

	~Player();
};

class Bullet {
public:
	Texture2D* texture;
	float power;

	Bullet(Vector3 position, float power);

	void Move(Vector3 velocity);

	void Render();

	~Bullet();
};

class Monster {
public:
	Texture2D* texture;
	Vector3 initialPos;
	float health;

	Monster(Vector3 initialPos, float health);

	void Render();

	void Move(Vector3 velocity);

	~Monster();
};

class Level {
public:
	std::string name;
	bool isEndLevel = false;
	std::vector<Monster*> monsters;

	Level(std::string name, bool isEndLevel);

	void SpwanMonsters(int noOfMonsters, float health);

	~Level();
};

// Scenes Models

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

class GameScene : public Scene {
private:
	Player* player;
	std::vector<Bullet*> bullets;
	std::vector<Bullet*> bulletsPool;
	std::vector<Bullet*> bulletsToRemove;
	ShaderProgram* shaderProgram = nullptr;
	unsigned int currentKeyPress = 0;
	Texture2D* background = nullptr;
	Level* currentLevel = nullptr;

	void FillInBulletsPool(int noOfBullets);

public:
	void Init();

	void ProcessInput(GLFWwindow* window);

	void Render();

	~GameScene();
};