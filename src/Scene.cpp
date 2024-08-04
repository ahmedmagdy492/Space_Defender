#include "Models.h"

void MenuScene::Init() {
}

void MenuScene::ProcessInput(GLFWwindow* window) {
}

void MenuScene::Render() {
}

MenuScene::~MenuScene() {

}

// Game Scene

void GameScene::Init() {
	Vector3 position;
	position.x = (SCREEN_WIDTH - PLAYER_SHIP_WIDTH) / 2;
	position.y = SCREEN_HEIGHT - PLAYER_SHIP_HEIGHT - 20;

	player = new Player(position);
}

void GameScene::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		currentKeyPress = GLFW_KEY_LEFT;
		if (player->texture->position.x > 3) {
			Vector3 velocity(-3 * 0.2, 0, 0);
			player->Move(velocity);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		currentKeyPress = GLFW_KEY_RIGHT;
		if ((player->texture->position.x + player->texture->width) < SCREEN_WIDTH) {
			Vector3 velocity(3 * 0.2, 0, 0);
			player->Move(velocity);
		}
	}
	if (currentKeyPress == 0 && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		currentKeyPress = GLFW_KEY_SPACE;
		Bullet* bullet = new Bullet(player->texture->position, 10);
		bullets.push_back(bullet);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		currentKeyPress = 0;
	}
}

void GameScene::Render() {
	for (auto& bullet : bullets) {
		bullet->Render();
		Vector3 velocity(0, -3 * 0.2, 0);
		bullet->Move(velocity);
	}

	player->Render();
}

GameScene::~GameScene() {
	delete player;
	player = nullptr;

	for (auto& bullet : bullets) {
		if (bullet) {
			delete bullet;
			bullet = nullptr;
		}
	}
}