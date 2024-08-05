#include "Models.h"

Player::Player(Vector3 position) : health(100) {
	texture = new Texture2D(position, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, "resources/ship.png", GL_TEXTURE0);
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
	texture = new Texture2D(position, BULLET_WIDTH, BULLET_HEIGHT, "resources/bullet.png", GL_TEXTURE1);
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
	if (texture) {
		delete texture;
		texture = nullptr;
	}
}

Monster::Monster(Vector3 curPosition, float health) : initialPos(curPosition), health(health) {
	texture = new Texture2D(curPosition, NORMAL_SHIP_WIDTH, NORMAL_SHIP_HEIGHT, "resources/monster.png", GL_TEXTURE3);
	texture->Init();
}

void Monster::Render() {
	texture->Draw();
}

void Monster::Move(Vector3 velocity) {
	if (texture->position.x <= (initialPos.x + (velocity.x * 5)) && texture->position.x >= initialPos.x) {
		texture->position.x += velocity.x;
		texture->position.y += velocity.y;
		texture->position.z += velocity.z;
	}
	else if(texture->position.x >= (initialPos.x - (velocity.x * 5))) {
		texture->position.x -= velocity.x;
		texture->position.y -= velocity.y;
		texture->position.z -= velocity.z;
	}
	texture->UpdateTexture(texture->position);
}

Monster::~Monster() {
	if (texture) {
		delete texture;
		texture = nullptr;
	}
}

Level::Level(std::string name, bool isEndLevel) : name(name), isEndLevel(isEndLevel) {
}

void Level::SpwanMonsters(int noOfMonsters, float health) {
	float x = 30, y = 30;
	float spaceBetweenObjs = 20;
	for (int i = 0; i < noOfMonsters; ++i) {
		Monster* monster = new Monster(Vector3(x, y, 0), health);
		monsters.push_back(monster);
		x += 30 + NORMAL_SHIP_WIDTH + spaceBetweenObjs;
		if (x >= SCREEN_WIDTH) {
			x = 30;
			// we should consider creating a reasonable no of objects to do not fill the whole screen with monsters
			y += NORMAL_SHIP_HEIGHT + spaceBetweenObjs;
		}
	}
}

Level::~Level() {
	for (auto& monster : monsters) {
		if (monster) {
			delete monster;
		}
	}

	monsters.clear();
}