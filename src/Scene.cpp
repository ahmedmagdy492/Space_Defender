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
		if (player->texture->position.x > 3) {
			Vector3 velocity(-3 * 0.2, 0, 0);
			player->Move(velocity);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if ((player->texture->position.x + player->texture->width) < SCREEN_WIDTH) {
			Vector3 velocity(3 * 0.2, 0, 0);
			player->Move(velocity);
		}
	}
}

void GameScene::Render() {
	player->Render();
}

GameScene::~GameScene() {
	delete player;
	player = nullptr;
}