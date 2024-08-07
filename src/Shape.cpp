#pragma once

#include "Models.h"


float Shape::ConvertToNDCForX(float x) {
	return ((2.0f * x) / SCREEN_WIDTH) - 1.0f;
}

float Shape::ConvertToNDCForY(float y) {
	return 1.0f - ((2.0f * y) / SCREEN_HEIGHT);
}

Rectangle::Rectangle(Vector3 position, int width, int height, Color color) : width(width), height(height) {
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
	glBindVertexArray(vaoId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Texture Shape
Texture2D::Texture2D(Vector3 position, int width, int height, Image* img, unsigned int textureUnit) : textureUnit(textureUnit), textureId(0) {
	this->image = img;
	this->position = position;
	this->width = width;
	this->height = height;
	this->textureUnit = textureUnit;
}

void PrintErrorIfThereAny(std::string msg) {
	GLenum errCode = glGetError();
	if (errCode != 0) {
		std::cout << "OPEN_GL::Error " << msg << ": " << errCode << std::endl;
	}
}

void Texture2D::SetOption(GLenum name, int value) {
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, name, value);
}

void Texture2D::Init() {
	SetupTextureBuffers(position);

	glGenTextures(1, &textureId);
	PrintErrorIfThereAny("After Generating Texture");
	glActiveTexture(textureUnit);
	PrintErrorIfThereAny("After Activating Texture Unit");
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, image->imgPixelFormat, image->width, image->height, 0, image->imgPixelFormat, GL_UNSIGNED_BYTE, image->data);
	PrintErrorIfThereAny("After Copying Texture Data");
	glGenerateMipmap(GL_TEXTURE_2D);
	PrintErrorIfThereAny("After Generating Mipmaps");
}

void Texture2D::SetupTextureBuffers(Vector3 position) {
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBuffer), elementBuffer, GL_STATIC_DRAW);
}

void Texture2D::UpdateTexture(Vector3 position) {
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

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexBuffer), vertexBuffer);
}

void Texture2D::Draw() {
	glBindVertexArray(vaoId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}