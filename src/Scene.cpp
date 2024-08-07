#include "Models.h"

bool IsObjectClicked(Vector3 pos, int width, int height, Vector3 mousePos) {
	return (mousePos.x >= pos.x && mousePos.x <= (pos.x + width)) && (mousePos.y >= pos.y && mousePos.y <= (pos.y + height));
}

SceneManager::SceneManager() {
	GameScene* gameScene = new GameScene(this);
	scenes.insert({ "GameScene", gameScene });
	MenuScene* menuScene = new MenuScene(this);
	scenes.insert({ "MenuScene", menuScene });
	currentActiveScene = menuScene;
}

void SceneManager::SwitchToScene(const std::string& sceneName) {
	std::unordered_map<std::string, Scene*>::iterator it = scenes.find(sceneName);
	if (it != scenes.end()) {
		currentActiveScene = scenes[it->first];
	}
}

SceneManager::~SceneManager() {
	delete scenes["GameScene"];
	delete scenes["MenuScene"];
}

MenuScene::MenuScene(SceneManager* sceneManager) {
	this->sceneManager = sceneManager;
	bgImg = new Image();
	bgImg->LoadImage("resources/bg.png");
	bgTexture = new Texture2D(Vector3(0, 0, 0), SCREEN_WIDTH, SCREEN_HEIGHT, bgImg, GL_TEXTURE2);
	bgTexture->Init();

	titleImg = new Image();
	titleImg->LoadImage("resources/title.png");
	titleTexture = new Texture2D(Vector3((SCREEN_WIDTH - titleImg->width)/2, 40, 0), titleImg->width, titleImg->height, titleImg, GL_TEXTURE7);
	titleTexture->Init();

	Init();

	btnImgs[0] = new Image();
	btnImgs[0]->LoadImage("resources/button1.png");
	btnImgs[1] = new Image();
	btnImgs[1]->LoadImage("resources/button2.png");

	int yPadding = 0;
	for (int i = 0; i < NO_OF_UI_BTNS; ++i) {
		imgBtns[i] = new ImageButton(
			Vector3(
				(SCREEN_WIDTH - BTN_WIDTH) / 2,
				(SCREEN_HEIGHT - BTN_HEIGHT) / 2 + yPadding,
				0.0f
			), 
			btnImgs[i], GL_TEXTURE5 + i);
		yPadding += BTN_HEIGHT - 50;
	}
}

void MenuScene::Init() {

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
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		throw std::string("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
}

void MenuScene::ProcessInput(GLFWwindow* window) {
	static bool isLeftMouseClicked = false;
	if (!isLeftMouseClicked && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		isLeftMouseClicked = true;
		double x = 0, y = 0;
		glfwGetCursorPos(window, &x, &y);

		if (IsObjectClicked(imgBtns[0]->texture->position, BTN_WIDTH, BTN_HEIGHT, Vector3((float)x, (float)y, 0.0f))) {
			std::string gameScene = "GameScene";
			sceneManager->SwitchToScene(gameScene);
		}
		else if (IsObjectClicked(imgBtns[1]->texture->position, BTN_WIDTH, BTN_HEIGHT, Vector3((float)x, (float)y, 0.0f))) {
			glfwSetWindowShouldClose(window, true);
		}
	}
	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
		isLeftMouseClicked = false;
	}
}

void MenuScene::Render() {
	shaderProgram->Use();
	shaderProgram->SetInt("inTexture", 2);
	bgTexture->Draw();

	for (int i = 0; i < NO_OF_UI_BTNS; ++i) {
		shaderProgram->SetInt("inTexture", 5+i);
		imgBtns[i]->Render();
	}

	shaderProgram->SetInt("inTexture", 7);
	titleTexture->Draw();
}

MenuScene::~MenuScene() {
	if (shaderProgram) {
		delete shaderProgram;
	}

	if (bgImg) {
		bgImg->UnloadImage();
		delete bgImg;
	}

	if (bgTexture) {
		delete bgTexture;
	}

	if (titleImg) {
		titleImg->UnloadImage();
		delete titleImg;
	}

	if (titleTexture) {
		delete titleTexture;
	}

	for (int i = 0; i < NO_OF_UI_BTNS; ++i) {
		if (btnImgs[i]) {
			btnImgs[i]->UnloadImage();
			delete btnImgs[i];
		}
	}

	for (int i = 0; i < NO_OF_UI_BTNS; ++i) {
		if (imgBtns[i]) {
			delete imgBtns[i];
		}
	}
}

// Game Scene

void GameScene::FillInBulletsPool(int noOfBullets = 100) {
	for (int i = 0; i < noOfBullets; ++i) {
		Bullet* bullet = new Bullet(player->texture->position, BULLET_POWER_LVL1, bulletImg);
		bulletsPool.push_back(bullet);
	}
}

void GameScene::FillInGrenadesPools(int noOfGrenades = 50) {
	for (int i = 0; i < noOfGrenades; ++i) {
		Grenade* grenade = new Grenade(Vector3(-100.0f, -100.0f, 0.0f), grenadeImg, BOMB_POWER_LVL1);
		bombsPool.push_back(grenade);
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
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		throw std::string("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}

	Vector3 playerPos;
	playerPos.x = (SCREEN_WIDTH - PLAYER_SHIP_WIDTH) / 2;
	playerPos.y = SCREEN_HEIGHT - PLAYER_SHIP_HEIGHT - 20;
	player = new Player(playerPos, playerImg);

	FillInBulletsPool();
	FillInGrenadesPools(100);

	currentLevel = new Level("Level", false);
	currentLevel->SpwanMonsters(50, LEVEL_1_MONSTERS_POWER);
	levelsFinished++;
}

GameScene::GameScene(SceneManager* sceneManager) {
	this->sceneManager = sceneManager;
	backgroundImg = new Image();
	backgroundImg->LoadImage("resources/bg.png");
	background = new Texture2D(Vector3(0, 0, 0), SCREEN_WIDTH, SCREEN_HEIGHT, backgroundImg, GL_TEXTURE2);
	background->Init();

	minus10Img = new Image();
	minus10Img->LoadImage("resources/minus_10.png");
	minus10 = new Texture2D(Vector3(-50, -50, 0), 30, 30, minus10Img, GL_TEXTURE4);
	minus10->Init();

	bulletImg = new Image();
	bulletImg->LoadImageW("resources/bullet.png");

	playerImg = new Image();
	playerImg->LoadImageW("resources/ship.png");

	grenadeImg = new Image();
	grenadeImg->LoadImage("resources/bomb.png");

	Init();
}

void GameScene::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		sceneManager->SwitchToScene("MenuScene");
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		currentKeyPress = GLFW_KEY_LEFT;
		if (player->texture->position.x > 3) {
			Vector3 velocity(-3.0f * 0.2f, 0.0f, 0.0f);
			player->Move(velocity);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		currentKeyPress = GLFW_KEY_RIGHT;
		if ((player->texture->position.x + player->texture->width) < SCREEN_WIDTH) {
			Vector3 velocity(3.0f * 0.2f, 0.0f, 0.0f);
			player->Move(velocity);
		}
	}
	if (currentKeyPress != GLFW_KEY_SPACE && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		currentKeyPress = GLFW_KEY_SPACE;
		if (bulletsPool.size() > 0) {
			Bullet* bullet = bulletsPool[bulletsPool.size()-1];
			bulletsPool.erase(bulletsPool.begin() + (bulletsPool.size() - 1));
			bullet->texture->position = player->texture->position;
			bullet->texture->UpdateTexture(player->texture->position);
			bullets.push_back(bullet);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		currentKeyPress = 0;
	}
}

bool GameScene::BulletCollidedWithMonster(Vector3 bullet, Vector3 monster) {
	if (currentLevel->isEndLevel) {
		return (bullet.x < monster.x + BIG_BOSS_WIDTH &&
			bullet.x + BULLET_WIDTH > monster.x &&
			bullet.y < monster.y + BIG_BOSS_HEGIHT &&
			bullet.y + BULLET_HEIGHT > monster.y);
	}
	else {
		return (bullet.x < monster.x + NORMAL_SHIP_WIDTH &&
			bullet.x + BULLET_WIDTH > monster.x &&
			bullet.y < monster.y + NORMAL_SHIP_HEIGHT &&
			bullet.y + BULLET_HEIGHT > monster.y);
	}
}

bool GameScene::BombCollidedWithPlayer(Vector3 bomb, Vector3 player) {
	return (bomb.x < player.x + NORMAL_SHIP_WIDTH &&
		bomb.x + GRENADE_WIDTH > player.x &&
		bomb.y < player.y + NORMAL_SHIP_HEIGHT &&
		bomb.y + GRENADE_HEIGHT > player.y);
}

void GameScene::Render() {
	shaderProgram->Use();
	shaderProgram->SetInt("inTexture", 2);
	background->Draw();

	for (auto& bombToRemove : bombsToRemove) {
		std::vector<Grenade*>::iterator it = std::find(bombs.begin(), bombs.end(), bombToRemove);
		if (it != bombs.end()) {
			bombs.erase(it);
		}
		bombsPool.push_back(bombToRemove);
	}
	bombsToRemove.clear();

	for (auto& monsterToBeRemoved : monstersToRemove) {
		std::vector<Monster*>::iterator it = std::find(currentLevel->monsters.begin(), currentLevel->monsters.end(), monsterToBeRemoved);
		if (it != currentLevel->monsters.end()) {
			currentLevel->monsters.erase(it);
		}
	}
	monstersToRemove.clear();

	for (auto& bulletToRemove : bulletsToRemove) {
		std::vector<Bullet*>::iterator it = std::find(bullets.begin(), bullets.end(), bulletToRemove);
		if (it != bullets.end()) {
			bullets.erase(it);
		}
		bulletsPool.push_back(bulletToRemove);
	}
	bulletsToRemove.clear();

	if (this->currentLevel->monsters.size() == 0) {
		bullets.clear();
		delete currentLevel;
 		if ((levelsFinished % 10) == 0) {
			currentLevel = new Level("Level", true);
			currentLevel->SpwanMonsters(1, LEVEL_10_MONSTERS_POWER * levelsFinished);
		}
		else {
			currentLevel = new Level("Level", false);
			currentLevel->SpwanMonsters(NO_OF_MONSTERS, LEVEL_1_MONSTERS_POWER * levelsFinished);
		}

		++levelsFinished;
		std::cout << "Changed to level " << levelsFinished << std::endl;
	}

	shaderProgram->SetInt("inTexture", 8);
	for (auto& bomb : bombs) {
		Vector3 velocity(0.0f, 5.0f * 0.2f, 0.0f);
		bomb->Move(velocity);
		bomb->Render();

		if ((bomb->texture->position.y + bomb->texture->height) > SCREEN_HEIGHT) {
			bomb->texture->position = Vector3(-100.0f, -100.0f, 0.0f);
			bomb->texture->UpdateTexture(bomb->texture->position);
			bombsToRemove.push_back(bomb);
		}
		else {
			if (BombCollidedWithPlayer(bomb->texture->position, player->texture->position)) {
				player->health -= bomb->power;
				if (player->health <= 0) {
					// TODO: Reset the current level
					std::cout << "Game Over: player health: " << player->health << std::endl;
				}
				else {
					minus10->position = player->texture->position;
					minus10->UpdateTexture(minus10->position);
				}
				bomb->texture->position = Vector3(-100.0f, -100.0f, 0.0f);
				bomb->texture->UpdateTexture(bomb->texture->position);
				bombsToRemove.push_back(bomb);
			}
		}
	}

	shaderProgram->SetInt("inTexture", 1);
	for (auto& bullet : bullets) {
		bullet->Render();
		Vector3 velocity(0.0f, -3.0f * 0.2f, 0.0f);
		bullet->Move(velocity);
		
		if ((bullet->texture->position.y + bullet->texture->height) < 0) {
			bulletsToRemove.push_back(bullet);
		}
		else {
			for (auto& monster : currentLevel->monsters) {
				if (BulletCollidedWithMonster(bullet->texture->position, monster->texture->position)) {
					monster->health -= bullet->power;
					if (monster->health <= 0) {
						monstersToRemove.push_back(monster);
					}
					else {
						minus10->position = monster->texture->position;
						minus10->UpdateTexture(minus10->position);
					}
					bullet->texture->position = Vector3(player->texture->position);
					bullet->texture->UpdateTexture(bullet->texture->position);
					bulletsToRemove.push_back(bullet);
					break;
				}
			}
		}
	}

	shaderProgram->SetInt("inTexture", 0);
	player->Render();


	shaderProgram->SetInt("inTexture", 3);
	for (auto& monster : currentLevel->monsters) {
		Vector3 velocity(.5f*.2f, 0.0f, 0.0f);
		monster->Move(velocity);
		monster->Render();
	}

	int time = (int)glfwGetTime();
	if ((time % 2) == 0) {
		if (currentLevel->monsters.size() > 0 && bombsPool.size() > 0) {
			Grenade* bomb = bombsPool[bombsPool.size() - 1];
			bombsPool.erase(bombsPool.begin() + (bombsPool.size() - 1));
			srand(time);
			int randMonsterIndex = rand() % currentLevel->monsters.size();
			bomb->texture->position = currentLevel->monsters[randMonsterIndex]->texture->position;
			bomb->texture->UpdateTexture(bomb->texture->position);
			bombs.push_back(bomb);
		}
	}

	shaderProgram->SetInt("inTexture", 4);
	minus10->position = Vector3(minus10->position.x, minus10->position.y-0.2f, 0.0f);
	minus10->UpdateTexture(minus10->position);
	minus10->Draw();
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

	for (auto& bomb : bombsPool) {
		if (bomb) {
			delete bomb;
			bomb = nullptr;
		}
	}

	if (background) {
		if (backgroundImg) {
			backgroundImg->UnloadImage();
			delete backgroundImg;
		}

		delete background;
	}

	if (minus10) {
		if (minus10Img) {
			minus10Img->UnloadImage();
			delete minus10Img;
		}

		delete minus10;
	}

	if (playerImg) {
		playerImg->UnloadImage();
		delete playerImg;
	}

	if (bulletImg) {
		bulletImg->UnloadImage();
		delete bulletImg;
	}

	if (grenadeImg) {
		grenadeImg->UnloadImage();
		delete grenadeImg;
	}

	if (currentLevel) {
		delete currentLevel;
	}
}