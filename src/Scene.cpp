#include "Models.h"
#include "MovementMechanisms.h"

bool IsObjectClicked(Vector3 pos, int width, int height, Vector3 mousePos) {
	return (mousePos.x >= pos.x && mousePos.x <= (pos.x + width)) && (mousePos.y >= pos.y && mousePos.y <= (pos.y + height));
}

SceneManager::SceneManager() {
	GameScene* gameScene = new GameScene(this);
	scenes.insert({ "GameScene", gameScene });
	MenuScene* menuScene = new MenuScene(this);
	scenes.insert({ "MenuScene", menuScene });
	TransitionScene* transitionScene = new TransitionScene(this);
	scenes.insert({ "TransitionScene", transitionScene });
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
	delete scenes["TransitionScene"];
}

MenuScene::MenuScene(SceneManager* sceneManager) {
	Config& config = Config::GetInstance();

	this->sceneManager = sceneManager;
	bgImg = new Image();
	bgImg->LoadImage("resources/bg.png");
	bgTexture = new Texture2D(Vector3(0, 0, 0), config.GetScreenWidth(), config.GetScreenHeight(), bgImg, GL_TEXTURE2);
	bgTexture->Init();

	titleImg = new Image();
	titleImg->LoadImage("resources/title.png");
	titleTexture = new Texture2D(Vector3((config.GetScreenWidth() - titleImg->width) / 2.0f, 100.0f, 0.0f), titleImg->width, titleImg->height, titleImg, GL_TEXTURE7);
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
				(config.GetScreenWidth() - BTN_WIDTH) / 2.0f,
				(config.GetScreenHeight() - BTN_HEIGHT) / 2.0f + yPadding,
				0.0f
			), 
			btnImgs[i], GL_TEXTURE5 + i);
		yPadding += BTN_HEIGHT + 30;
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
	Config& config = Config::GetInstance();

	static bool isLeftMouseClicked = false;
	if (!isLeftMouseClicked && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		isLeftMouseClicked = true;
		double x = 0, y = 0;
		glfwGetCursorPos(window, &x, &y);

		if (IsObjectClicked(Vector3((config.GetScreenWidth() - BTN_WIDTH) / 2.0f, (config.GetScreenHeight() - BTN_HEIGHT) / 2.0f, 0.0f), BTN_WIDTH, BTN_HEIGHT, Vector3((float)x, (float)y, 0.0f))) {
			std::string gameScene = "GameScene";
			sceneManager->SwitchToScene(gameScene);
		}
		else if (IsObjectClicked(Vector3((config.GetScreenWidth() - BTN_WIDTH) / 2.0f, (config.GetScreenHeight() - BTN_HEIGHT) / 2.0f + BTN_HEIGHT + 30.0f, 0.0f), BTN_WIDTH, BTN_HEIGHT, Vector3((float)x, (float)y, 0.0f))) {
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
		bullet->power = player->shootPower;
		bulletsPool.push_back(bullet);
	}
}

void GameScene::FillInGrenadesPools(int noOfGrenades = 50) {
	for (int i = 0; i < noOfGrenades; ++i) {
		Grenade* grenade = new Grenade(Vector3(-100.0f, -100.0f, 0.0f), grenadeImg, BOMB_POWER_LVL1);
		bombsPool.push_back(grenade);
	}
}

void GameScene::ResetScene() {
	Config& config = Config::GetInstance();

	bulletsPool.clear();
	bombsPool.clear();
	bombs.clear();
	bullets.clear();
	currentLevel->monsters.clear();

	player->ResetScore();
	player->ZeroHealth();
	player->IncreaseHealth(150);
	Vector3 playerPos;
	playerPos.x = (config.GetScreenWidth() - PLAYER_SHIP_WIDTH) / 2.0f;
	playerPos.y = config.GetScreenHeight() - PLAYER_SHIP_HEIGHT - 20.0f;
	player->texture->position = playerPos;
	player->healthBar->rectangle->UpdateShape(player->healthBar->rectangle->position);

	FillInBulletsPool();
	FillInGrenadesPools(100);

	// TODO: need to adjust the monsters power here
	currentLevel->SpwanMonsters(20, LEVEL_1_MONSTERS_POWER);
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

	Config& config = Config::GetInstance();

	Vector3 playerPos;
	playerPos.x = (config.GetScreenWidth() - PLAYER_SHIP_WIDTH) / 2.0f;
	playerPos.y = config.GetScreenHeight() - PLAYER_SHIP_HEIGHT - 20.0f;
	player = new Player(playerPos, playerImg);
	playerImg->UnloadImage();
	delete playerImg;

	FillInBulletsPool();
	FillInGrenadesPools(100);

	currentLevel = new Level("Level", false);
	currentLevel->SpwanMonsters(20, LEVEL_1_MONSTERS_POWER);
	levelsFinished++;
}

GameScene::GameScene(SceneManager* sceneManager) {
	Config& config = Config::GetInstance();

	this->sceneManager = sceneManager;
	backgroundImg = new Image();
	backgroundImg->LoadImage("resources/bg.png");
	background = new Texture2D(Vector3(0, 0, 0), config.GetScreenWidth(), config.GetScreenHeight(), backgroundImg, GL_TEXTURE2);
	background->Init();
	backgroundImg->UnloadImage();
	delete backgroundImg;

	minus10Img = new Image();
	minus10Img->LoadImage("resources/minus_10.png");
	minus10 = new Texture2D(Vector3(-50, -50, 0), 30, 30, minus10Img, GL_TEXTURE4);
	minus10->Init();
	minus10Img->UnloadImage();
	delete minus10Img;

	bulletImg = new Image();
	bulletImg->LoadImageW("resources/bullet.png");

	playerImg = new Image();
	playerImg->LoadImageW("resources/ship.png");

	grenadeImg = new Image();
	grenadeImg->LoadImage("resources/bomb.png");

	explodeImg = new Image();
	explodeImg->LoadImage("resources/explode.png");
	explodeTexture = new Texture2D(Vector3(-100.0f, -100.0f, 0.0f), GRENADE_WIDTH, GRENADE_HEIGHT, explodeImg, GL_TEXTURE9);
	explodeTexture->Init();
	explodeImg->UnloadImage();
	delete explodeImg;

	bigBossImg = new Image();
	bigBossImg->LoadImage("resources/bigboss.png");
	bigBossTexture = new Texture2D(Vector3(0.0f, 0.0f, 0.0f), BIG_BOSS_WIDTH, BIG_BOSS_HEGIHT, bigBossImg, GL_TEXTURE11);
	bigBossTexture->Init();
	bigBossImg->UnloadImage();
	delete bigBossImg;

	Init();

	bulletLv2Img = new Image();
	bulletLv2Img->LoadImage("resources/bullet_lv2.png");
	bulletLv2Texture = new Texture2D(player->texture->position, BULLET_WIDTH, BULLET_HEIGHT, bulletLv2Img, GL_TEXTURE10);
	bulletLv2Texture->Init();
	bulletLv2Img->UnloadImage();
	delete bulletLv2Img;
}

void GameScene::ShootBullet(Vector3 bulletTriggerPos) {
	Bullet* bullet = bulletsPool[bulletsPool.size() - 1];
	bulletsPool.erase(bulletsPool.begin() + (bulletsPool.size() - 1));
	bullet->texture->position = bulletTriggerPos;
	bullet->texture->UpdateTexture(player->texture->position);
	bullets.push_back(bullet);
}

void GameScene::ProcessInput(GLFWwindow* window) {
	Config& config = Config::GetInstance();

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
		if ((player->texture->position.x + player->texture->width) < config.GetScreenWidth()) {
			Vector3 velocity(3.0f * 0.2f, 0.0f, 0.0f);
			player->Move(velocity);
		}
	}
	int time = (int)glfwGetTime();
	static int lastShootTime = -1;
	if (currentKeyPress != GLFW_KEY_SPACE && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		currentKeyPress = GLFW_KEY_SPACE;
		if (bulletsPool.size() > 0 && lastShootTime != time) {
			lastShootTime = time;
			switch (player->currentShootingSkill) {
				case PlayerShootingSkills::DualShootingLevel1: {
					ShootBullet(player->texture->position);
					ShootBullet(Vector3(player->texture->position.x + PLAYER_SHIP_WIDTH - BULLET_WIDTH, player->texture->position.y, player->texture->position.z));
					break;
				}
				case PlayerShootingSkills::TripleShootingLevel1: {
					ShootBullet(player->texture->position);
					ShootBullet(Vector3(
						player->texture->position.x + ((PLAYER_SHIP_WIDTH - BULLET_WIDTH) / 2),
						player->texture->position.y, 
						player->texture->position.z)
					);
					ShootBullet(Vector3(player->texture->position.x + PLAYER_SHIP_WIDTH - BULLET_WIDTH, player->texture->position.y, player->texture->position.z));
					break;
				}
				case PlayerShootingSkills::SingleShootingLevel2: {
					ShootBullet(player->texture->position);
					break;
				}
				case PlayerShootingSkills::DualShootingLevel2: {
					ShootBullet(player->texture->position);
					ShootBullet(Vector3(player->texture->position.x + PLAYER_SHIP_WIDTH - BULLET_WIDTH, player->texture->position.y, player->texture->position.z));
					break;
				}
				default: {
					ShootBullet(Vector3(
						player->texture->position.x + ((PLAYER_SHIP_WIDTH - BULLET_WIDTH) / 2),
						player->texture->position.y,
						player->texture->position.z)
					);
					ShootBullet(Vector3(player->texture->position.x + PLAYER_SHIP_WIDTH - BULLET_WIDTH, player->texture->position.y, player->texture->position.z));
					break;
				}
			}
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
	Config& config = Config::GetInstance();

	shaderProgram->Use();
	shaderProgram->SetInt("inTexture", 2);
	background->Draw();

	for (auto bombToRemove : bombsToRemove) {
		std::vector<Grenade*>::iterator it = std::find(bombs.begin(), bombs.end(), bombToRemove);
		if (it != bombs.end()) {
			bombs.erase(it);
		}
		bombsPool.push_back(bombToRemove);
	}
	bombsToRemove.clear();

	for (auto monsterToBeRemoved : monstersToRemove) {
		std::vector<Monster*>::iterator it = std::find(currentLevel->monsters.begin(), currentLevel->monsters.end(), monsterToBeRemoved);
		if (it != currentLevel->monsters.end()) {
			currentLevel->monsters.erase(it);
		}
	}
	monstersToRemove.clear();

	for (auto bulletToRemove : bulletsToRemove) {
		std::vector<Bullet*>::iterator it = std::find(bullets.begin(), bullets.end(), bulletToRemove);
		if (it != bullets.end()) {
			bullets.erase(it);
		}
		bulletsPool.push_back(bulletToRemove);
	}
	bulletsToRemove.clear();

	if (this->currentLevel->monsters.size() == 0) {
		bullets.clear();
		bombs.clear();
		delete currentLevel;
 		if ((levelsFinished % 5) == 0) {
			currentLevel = new Level("Level", true);
			currentLevel->SpwanMonsters(1, LEVEL_10_MONSTERS_POWER * levelsFinished);
		}
		else {
			currentLevel = new Level("Level", false);
			currentLevel->SpwanMonsters((NO_OF_MONSTERS - levelsFinished) > 20 ? NO_OF_MONSTERS - levelsFinished : NO_OF_MONSTERS - 20, LEVEL_1_MONSTERS_POWER * levelsFinished);
		}

		++levelsFinished;
		sceneManager->SwitchToScene("TransitionScene");
	}

	if (currentLevel->isEndLevel) {
		shaderProgram->SetInt("inTexture", 9);
	}
	else {
		shaderProgram->SetInt("inTexture", 8);
	}
	for (auto bomb : bombs) {
		Vector3 velocity(0.0f, 5.0f * 0.2f, 0.0f);
		bomb->Move(velocity);
		bomb->Render();

		if ((bomb->texture->position.y + bomb->texture->height) > config.GetScreenHeight()) {
			bomb->texture->position = Vector3(-100.0f, -100.0f, 0.0f);
			bomb->texture->UpdateTexture(bomb->texture->position);
			bombsToRemove.push_back(bomb);
		}
		else {
			if (BombCollidedWithPlayer(bomb->texture->position, player->texture->position)) {
				player->DecreaseHealth(bomb->power);
				if (player->GetHealth() <= 0) {
					ResetScene();
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

	switch (player->currentShootingSkill) {
	case PlayerShootingSkills::DualShootingLevel1:
	case PlayerShootingSkills::TripleShootingLevel1:
		shaderProgram->SetInt("inTexture", 1);
		break;
	default:
		shaderProgram->SetInt("inTexture", 10);
		break;
	}
	for (auto bullet : bullets) {
		bullet->Render();
		Vector3 velocity(0.0f, -3.0f * 0.2f, 0.0f);
		bullet->Move(velocity);
		
		if ((bullet->texture->position.y + bullet->texture->height) < 0) {
			bulletsToRemove.push_back(bullet);
		}
		else {
			for (auto monster : currentLevel->monsters) {
				if (BulletCollidedWithMonster(bullet->texture->position, monster->texture->position)) {
					monster->health -= bullet->power;
					if (monster->health <= 0) {
						monstersToRemove.push_back(monster);
					}
					else {
						player->IncreaseScore();
						std::cout << "Player Score: " << player->GetCurrentPlayerScore() << std::endl;
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

	shaderProgram->SetBool("isShape", true);
	shaderProgram->SetVector3("shapeColor", Vector3(player->healthBar->color.red, player->healthBar->color.green, player->healthBar->color.blue));
	player->healthBar->Render();
	shaderProgram->SetBool("isShape", false);

	if (!currentLevel->isEndLevel) {
		shaderProgram->SetInt("inTexture", 3);
	}
	else {
		shaderProgram->SetInt("inTexture", 11);
	}
	int counter = 0;
	for (auto monster : currentLevel->monsters) {
		if ((levelsFinished % 3) == 0 && !currentLevel->isEndLevel) {
			if ((counter % 2) == 0) {
				Vector3 velocity(.5f * .3f, 0.0f, 0.0f);
				monster->Move(velocity, MoveInOppositeDirections);
				monster->Render();
			}
			else {
				Vector3 velocity(.5f * .2f, 0.0f, 0.0f);
				monster->Move(velocity, MoveLeftToRightAndViceVersa);
				monster->Render();
			}
		}
		else {
			Vector3 velocity(.5f * .2f, 0.0f, 0.0f);
			monster->Move(velocity, MoveLeftToRightForEndLevel);
			monster->Render();
		}
		++counter;
	}

	int time = (int)glfwGetTime();
	static int lastBombSpwanTime = -1;
	if (time != lastBombSpwanTime) {
		lastBombSpwanTime = time;
		if (currentLevel->monsters.size() > 0 && bombsPool.size() > 0) {
			Grenade* bomb = bombsPool[bombsPool.size() - 1];
			bombsPool.erase(bombsPool.begin() + (bombsPool.size() - 1));
			srand(time);
			int randMonsterIndex = rand() % currentLevel->monsters.size();
			if (!currentLevel->isEndLevel) {
				bomb->texture->position = Vector3(
					currentLevel->monsters[randMonsterIndex]->texture->position.x + ((NORMAL_SHIP_WIDTH - GRENADE_WIDTH) / 2),
					currentLevel->monsters[randMonsterIndex]->texture->position.y,
					currentLevel->monsters[randMonsterIndex]->texture->position.z
				);
			}
			else {
				bomb->texture->position = Vector3(
					currentLevel->monsters[randMonsterIndex]->texture->position.x + ((BIG_BOSS_WIDTH - GRENADE_WIDTH) / 2),
					currentLevel->monsters[randMonsterIndex]->texture->position.y,
					currentLevel->monsters[randMonsterIndex]->texture->position.z
				);
			}
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

	for (auto bullet : bulletsPool) {
		if (bullet) {
			delete bullet;
			bullet = nullptr;
		}
	}

	for (auto bomb : bombsPool) {
		if (bomb) {
			delete bomb;
			bomb = nullptr;
		}
	}

	if (background) {
		delete background;
	}

	if (minus10) {
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

	if (explodeTexture) {
		delete explodeTexture;
	}

	if (bulletLv2Texture) {
		delete bulletLv2Texture;
	}
	
	if (bigBossTexture) {
		delete bigBossTexture;
	}

	if (currentLevel) {
		delete currentLevel;
	}
}



TransitionScene::TransitionScene(SceneManager* sceneManager) {
	Config& config = Config::GetInstance();

	this->sceneManager = sceneManager;
	nextLevelImg = new Image();
	nextLevelImg->LoadImage("resources/transition_scene.png");
	nextLevelTexture = new Texture2D(Vector3((config.GetScreenWidth() - nextLevelImg->width) / 2.0f, (config.GetScreenHeight() - nextLevelImg->height) / 2.0f, 0.0f), nextLevelImg->width, nextLevelImg->height, nextLevelImg, GL_TEXTURE12);
	nextLevelTexture->Init();
	nextLevelImg->UnloadImage();
	delete nextLevelImg;

	Image* pressEnterImg = new Image();
	pressEnterImg->LoadImage("resources/press_enter.png");
	pressEnterTexture = new Texture2D(Vector3((config.GetScreenWidth() - pressEnterImg->width) / 2.0f, (config.GetScreenHeight() - pressEnterImg->height) / 2.0f + nextLevelTexture->height + 10, 0.0f), pressEnterImg->width, pressEnterImg->height, pressEnterImg, GL_TEXTURE13);
	pressEnterTexture->Init();
	pressEnterImg->UnloadImage();
	delete pressEnterImg;

	Init();
}

void TransitionScene::Init() {
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

void TransitionScene::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		sceneManager->SwitchToScene("GameScene");
	}
}

void TransitionScene::Render() {
	shaderProgram->Use();
	shaderProgram->SetInt("inTexture", 12);
	nextLevelTexture->Draw();

	shaderProgram->SetInt("inTexture", 13);
	pressEnterTexture->Draw();
}

TransitionScene::~TransitionScene() {
	if (nextLevelTexture) {
		delete nextLevelTexture;
	}

	if (pressEnterTexture) {
		delete pressEnterTexture;
	}
}