#pragma once

#include "Models.h"

Shape::Shape() {
	try {
		std::ifstream vertexIfStream;
		std::ifstream fragmentIfStream;
		vertexIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::string vertexShaderPath = "shaders/default_vertex_shader.vert";
		std::string fragmentShaderPath = "shaders/default_fragment_shader.frag";
									
		vertexIfStream.open(vertexShaderPath);
		fragmentIfStream.open(fragmentShaderPath);
		std::stringstream vertexStrStream, fragmentStrStream;

		vertexStrStream << vertexIfStream.rdbuf();
		fragmentStrStream << fragmentIfStream.rdbuf();

		std::string vertexShaderSrc = vertexStrStream.str();
		std::string fragmentShaderSrc = fragmentStrStream.str();

		vertexIfStream.close();
		fragmentIfStream.close();

		shaderProgram = new ShaderProgram(vertexShaderSrc, fragmentShaderSrc);
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		throw std::string("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
}

Shape::~Shape() {
	delete shaderProgram;
	shaderProgram = nullptr;
}

float Rectangle::ConvertToNDCForX(float x) {
	return ((2.0f * x) / SCREEN_WIDTH) - 1.0f;
}

float Rectangle::ConvertToNDCForY(float y) {
	return 1.0f - ((2.0f * y) / SCREEN_HEIGHT);
}

Rectangle::Rectangle(Vector3 position, float width, float height, Color color) : width(width), height(height) {
	this->position = position;
	this->color = color;
}

void Rectangle::Init() {
	glGenBuffers(1, &vboId);

	Vector3 ndcTopLeft, ndcTopRight, ndcBottomLeft, ndcBottomRight;
	ndcTopLeft.x = ConvertToNDCForX(position.x);
	ndcTopLeft.y = ConvertToNDCForY(position.y);
	ndcTopLeft.z = 0.0f;

	ndcTopRight.x = ConvertToNDCForX(position.x + width);
	ndcTopRight.y = ConvertToNDCForY(position.y);
	ndcTopRight.z = 0.0f;

	ndcBottomLeft.x = ConvertToNDCForX(position.x);
	ndcBottomLeft.y = ConvertToNDCForY(position.y + height);
	ndcBottomLeft.z = 0.0f;

	ndcBottomRight.x = ConvertToNDCForX(position.x + width);
	ndcBottomRight.y = ConvertToNDCForY(position.y + height);
	ndcBottomRight.z = 0.0f;

	float vertexBuffer[] = {
		ndcTopLeft.x,     ndcTopLeft.y,    ndcTopLeft.z, color.red, color.green, color.blue,
		ndcTopRight.x,    ndcTopRight.y,   ndcTopLeft.z, color.red, color.green, color.blue,
		ndcBottomLeft.x,  ndcBottomLeft.y, ndcTopLeft.z, color.red, color.green, color.blue,
		ndcBottomRight.x, ndcBottomRight.y, ndcTopLeft.z, color.red, color.green, color.blue
	};

	float elementBuffer[] = {
		0, 2, 3,
		0, 1, 3
	};

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBuffer), elementBuffer, GL_STATIC_DRAW);
}

void Rectangle::Draw() {
	shaderProgram->Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}