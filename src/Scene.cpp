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

void GameScene::FillInBulletsPool(int noOfBullets = 100) {
	for (int i = 0; i < noOfBullets; ++i) {
		Bullet* bullet = new Bullet(Vector3(0, 0, 0), 10);
		bulletsPool.push_back(bullet);
	}
}

void GameScene::Init() {
	try {
		std::ifstream vertexIfStream;
		std::ifstream fragmentIfStream;
		vertexIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		vertexIfStream.open("shaders/texture_vertex_shader.vert");
		fragmentIfStream.open("shaders/texture_fragment_shader.frag");
		std::stringstream vertexStrStream, fragmentStrStream;

		vertexStrStream << vertexIfStream.rdbuf();
		fragmentStrStream << fragmentIfStream.rdbuf();

		std::string vertexShaderSrc = vertexStrStream.str();
		std::string fragmentShaderSrc = fragmentStrStream.str();

		vertexIfStream.close();
		fragmentIfStream.close();

		shaderProgram = new ShaderProgram(vertexShaderSrc, fragmentShaderSrc);

		std::cout << "Shader Program Created: " << shaderProgram->GetProgramId() << std::endl;

		background = new Texture2D(Vector3(0, 0, 0), SCREEN_WIDTH, SCREEN_HEIGHT, "resources/bg.png", GL_TEXTURE2);
		background->Init();

		FillInBulletsPool();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		throw std::string("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}

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
	if (currentKeyPress != GLFW_KEY_SPACE && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		currentKeyPress = GLFW_KEY_SPACE;
		if (bulletsPool.size() > 0) {
			Bullet* bullet = bulletsPool[bulletsPool.size()-1];
			bulletsPool.erase(bulletsPool.begin() + (bulletsPool.size() - 1));
			bullet->texture->position = player->texture->position;
			bullets.push_back(bullet);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		currentKeyPress = 0;
	}
}

void GameScene::Render() {
	shaderProgram->Use();
	shaderProgram->SetInt("inTexture", 2);
	background->Draw();

	shaderProgram->SetInt("inTexture", 1);

	for (auto& bulletToRemove : bulletsToRemove) {
		std::vector<Bullet*>::iterator it = std::find(bullets.begin(), bullets.end(), bulletToRemove);
		if (it != bullets.end()) {
			bullets.erase(it);
		}
		bulletsPool.push_back(bulletToRemove);
	}

	bulletsToRemove.clear();

	for (auto& bullet : bullets) {
		bullet->Render();
		Vector3 velocity(0, -3 * 0.2, 0);
		bullet->Move(velocity);
		if ((bullet->texture->position.y + bullet->texture->height) < 0) {
			bulletsToRemove.push_back(bullet);
		}
	}

	shaderProgram->SetInt("inTexture", 0);
	player->Render();
}

GameScene::~GameScene() {
	if (shaderProgram) {
		delete shaderProgram;
		shaderProgram = nullptr;
	}

	delete player;
	player = nullptr;

	for (auto& bullet : bulletsPool) {
		if (bullet) {
			delete bullet;
			bullet = nullptr;
		}
	}

	if (background) {
		delete background;
	}
}