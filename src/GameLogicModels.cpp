#include "Models.h"

Player::Player(Vector3 position, Image* img) : health(100) {
	texture = new Texture2D(position, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, img, GL_TEXTURE0);
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

// Bullet methods
Bullet::Bullet(Vector3 position, float power, Image* img) : power(power) {
	texture = new Texture2D(position, BULLET_WIDTH, BULLET_HEIGHT, img, GL_TEXTURE1);
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


Monster::Monster(Vector3 curPosition, float health, Image* img) : initialPos(curPosition), health(health) {
	texture = new Texture2D(curPosition, NORMAL_SHIP_WIDTH, NORMAL_SHIP_HEIGHT, img, GL_TEXTURE3);
	texture->Init();
}

void Monster::Render() {
	texture->Draw();
}

void Monster::Move(Vector3 velocity) {
	if (direction == MonsterMovingDirection::Right) {
		if (texture->position.x <= (initialPos.x + (velocity.x * 5005))) {
			texture->position.x += velocity.x;
			texture->position.y += velocity.y;
			texture->position.z += velocity.z;
		}
		else {
			direction = MonsterMovingDirection::Left;
		}
	}
	else if (direction == MonsterMovingDirection::Left) {
		if (texture->position.x >= (initialPos.x - (velocity.x * 5005))) {
			texture->position.x -= velocity.x;
			texture->position.y -= velocity.y;
			texture->position.z -= velocity.z;
		}
		else {
			direction = MonsterMovingDirection::Right;
		}
	}
	texture->UpdateTexture(texture->position);
}

Level::Level(std::string name, bool isEndLevel) : name(name), isEndLevel(isEndLevel) {
	monsterImg = new Image();
	monsterImg->LoadImage("resources/monster.png");
}

void Level::SpwanMonsters(int noOfMonsters, float health) {
	if (isEndLevel) {
		Monster* monster = new Monster(Vector3((SCREEN_WIDTH - 200) / 2, 30, 0), health, monsterImg);
		monster->texture->width = BIG_BOSS_WIDTH;
		monster->texture->height = BIG_BOSS_HEGIHT;
		monster->texture->UpdateTexture(monster->texture->position);
		monsters.push_back(monster);
	}
	else {
		int xOffset = 60;
		float x = 30, y = 30;
		float spaceBetweenObjs = 20;
		for (int i = 0; i < noOfMonsters; ++i) {
			Monster* monster = new Monster(Vector3(x, y, 0), health, monsterImg);
			monsters.push_back(monster);
			x += xOffset + NORMAL_SHIP_WIDTH;
			if (x >= SCREEN_WIDTH) {
				x = 30;
				// we should consider creating a reasonable no of objects to do not fill the whole screen with monsters
				y += NORMAL_SHIP_HEIGHT + spaceBetweenObjs;
			}
		}
	}
}

Level::~Level() {
	if (monsterImg) {
		monsterImg->UnloadImage();
		delete monsterImg;
	}

	for (auto& monster : monsters) {
		if (monster) {
			delete monster;
		}
	}

	monsters.clear();
}


Grenade::Grenade(Vector3 position, Image* img, int power) : power(power) {
	texture = new Texture2D(position, GRENADE_WIDTH, GRENADE_HEIGHT, img, GL_TEXTURE8);
	texture->Init();
}

void Grenade::Render() {
	texture->Draw();
}

void Grenade::Move(Vector3 velocity) {
	this->texture->position.y += velocity.y;
	this->texture->UpdateTexture(this->texture->position);
}

Grenade::~Grenade() {
	if (texture) {
		delete texture;
	}
}