#include "Models.h"

Player::Player(Vector3 position) : health(100) {
	texture = new Texture2D(position, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, "resources/ship.png", GL_RGBA, GL_TEXTURE0);
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

// Bullet methods
Bullet::Bullet(Vector3 position, float power) : power(power) {
	texture = new Texture2D(position, BULLET_WIDTH, BULLET_HEIGHT, "resources/bullet.png", GL_RGBA, GL_TEXTURE1);
	texture->Init();
}

void Bullet::Move(Vector3 velocity) {
	texture->position.x += velocity.x;
	texture->position.y += velocity.y;
	texture->position.z += velocity.z;
	texture->UpdateTexture(texture->position);
}

void Bullet::Render() {
	texture->Draw();
}

Bullet::~Bullet() {
	delete texture;
	texture = nullptr;
}