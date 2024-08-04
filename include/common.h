#pragma once

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

struct Color {
	float red;
	float green;
	float blue;
	float alpha;

	Color() : red(0), green(0), blue(0), alpha(0) {

	}

	Color(float red, float green, float blue) : red(red), green(green), blue(blue), alpha(1.0f) {

	}
};

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3() : x(0), y(0), z(0) {

	}

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {
	}
};