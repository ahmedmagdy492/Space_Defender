#include "Models.h"

Player::Player(Vector3 position) {
	texture = new Texture2D(position, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, "resources/ship.png", GL_RGBA);
	texture->Init();
}

void Player::Move(Vector3 velocity) {
	texture->position.x += velocity.x;
	texture->position.y += velocity.y;
	texture->position.z += velocity.z;
	texture->UpdateTexture(texture->position);
}

void Player::Render() {
	texture->Draw();
}

Player::~Player() {
	delete texture;
	texture = nullptr;
}