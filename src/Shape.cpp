#pragma once

#include "Models.h"

static int textureUnit = 0;

Shape::Shape(std::string vertexShaderPath, std::string fragmentShaderPath) {
	try {
		std::ifstream vertexIfStream;
		std::ifstream fragmentIfStream;
		vertexIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

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

float Shape::ConvertToNDCForX(float x) {
	return ((2.0f * x) / SCREEN_WIDTH) - 1.0f;
}

float Shape::ConvertToNDCForY(float y) {
	return 1.0f - ((2.0f * y) / SCREEN_HEIGHT);
}

Rectangle::Rectangle(Vector3 position, int width, int height, Color color) : width(width), height(height), Shape("shaders/default_vertex_shader.vert", "shaders/default_fragment_shader.frag") {
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

	unsigned int elementBuffer[] = {
		0, 2, 3,
		0, 1, 3
	};

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBuffer), elementBuffer, GL_DYNAMIC_DRAW);
}

void Rectangle::Draw() {
	shaderProgram->Use();
	glBindVertexArray(vaoId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Texture Shape
Texture2D::Texture2D(Vector3 position, int width, int height, std::string imagePath, GLenum imgPixelFormat) : imgWidth(0), imgHeight(0), Shape("shaders/texture_vertex_shader.vert", "shaders/texture_fragment_shader.frag") {
	this->imagePath = imagePath;
	this->position = position;
	this->imgPixelFormat = imgPixelFormat;
	this->width = width;
	this->height = height;
}

void Texture2D::SetOption(GLenum name, int value) {
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, name, value);
}

void Texture2D::Init() {
	int nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath.c_str(), &imgWidth, &imgHeight, &nChannels, 0);

	if (!data)
		throw new std::string("Unable to load provided image");

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
		ndcTopLeft.x,     ndcTopLeft.y,     ndcTopLeft.z,     0.0f, 1.0f,
		ndcTopRight.x,    ndcTopRight.y,    ndcTopRight.z,    1.0f, 1.0f,
		ndcBottomLeft.x,  ndcBottomLeft.y,  ndcBottomLeft.z,  0.0f, 0.0f,
		ndcBottomRight.x, ndcBottomRight.y, ndcBottomRight.z, 1.0f, 0.0f
	};

	unsigned int elementBuffer[] = {
		0, 2, 3,
		0, 1, 3
	};

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBuffer), elementBuffer, GL_DYNAMIC_DRAW);

	glGenTextures(1, &textureId);
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	++textureUnit;
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, imgPixelFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void Texture2D::Draw() {
	shaderProgram->Use();
	shaderProgram->SetInt("inTexture", 0);

	glBindVertexArray(vaoId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}