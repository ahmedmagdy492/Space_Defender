#pragma once

#define PLAYER_SHIP_WIDTH  80
#define PLAYER_SHIP_HEIGHT 50

#define NORMAL_SHIP_WIDTH  50
#define NORMAL_SHIP_HEIGHT 50

#define BIG_BOSS_WIDTH 150
#define BIG_BOSS_HEGIHT 150

#define BULLET_WIDTH 25
#define BULLET_HEIGHT 25

#define GRENADE_WIDTH 35
#define GRENADE_HEIGHT 25

#define BULLET_POWER_LVL1 10.0f
#define BULLET_POWER_LVL2 20.0f
#define BULLET_POWER_LVL3 30.0f
#define BULLET_POWER_LVL4 40.0f
#define BULLET_POWER_LVL5 50.0f

#define BOMB_POWER_LVL1 10
#define BOMB_POWER_LVL2 20
#define BOMB_POWER_LVL3 30
#define BOMB_POWER_LVL4 40
#define BOMB_POWER_LVL5 50

#define LEVEL_1_MONSTERS_POWER 20.0f
#define LEVEL_10_MONSTERS_POWER 5200.0f

#define NO_OF_MONSTERS 50

#define NO_OF_UI_BTNS 2
#define BTN_WIDTH 300
#define BTN_HEIGHT 48

#define HEALTH_BAR_HEIGHT 10

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

struct Vector2 {
	float x;
	float y;

	Vector2() : x(0), y(0) {

	}

	Vector2(float x, float y) : x(x), y(y) {
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