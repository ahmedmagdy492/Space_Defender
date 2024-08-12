#include "Models.h"

HealthBar::HealthBar(Vector3 position, int health) : health(health) {
	rectangle = new RectangleShape(position, health / 2, HEALTH_BAR_HEIGHT, color);
	rectangle->Init();
}

void HealthBar::Move(Vector3 velocity) {
	rectangle->UpdateShape(velocity);
}

void HealthBar::Render() {
	rectangle->Draw();
}

HealthBar::~HealthBar() {
	if (rectangle) {
		delete rectangle;
	}
}

Player::Player(Vector3 position, Image* img) : health(150) {
	texture = new Texture2D(position, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, img, GL_TEXTURE0);
	texture->Init();
	healthBar = new HealthBar(Vector3(position.x, position.y - HEALTH_BAR_HEIGHT - 10, 0.0f), health);
}

void Player::Move(Vector3 velocity) {
	Config& config = Config::GetInstance();

	texture->position.x += velocity.x;
	texture->position.y = config.GetScreenHeight() - PLAYER_SHIP_HEIGHT - 20.0f;
	texture->position.z += velocity.z;
	texture->UpdateTexture(texture->position);

	healthBar->rectangle->position.x = texture->position.x;
	healthBar->rectangle->position.y = texture->position.y - HEALTH_BAR_HEIGHT - 10.0f;
	healthBar->Move(healthBar->rectangle->position);
}

void Player::Render() {
	texture->Draw();
}

Player::~Player() {
	if (healthBar) {
		delete healthBar;
	}

	if (texture) {
		delete texture;
	}
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

void Monster::Move(Vector3 velocity, void (*MovementMechanism)(Monster* monster, Vector3 velocity)) {
	MovementMechanism(this, velocity);
}

Level::Level(std::string name, bool isEndLevel) : name(name), isEndLevel(isEndLevel) {
	monsterImg = new Image();
	monsterImg->LoadImage("resources/monster.png");
}

void Level::SpwanMonsters(int noOfMonsters, float health) {
	Config& config = Config::GetInstance();

	if (isEndLevel) {
		Monster* monster = new Monster(Vector3((config.GetScreenWidth() - 200.0f) / 2.0f, 30.0f, 0.0f), health, monsterImg);
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
			if (x >= config.GetScreenWidth()) {
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