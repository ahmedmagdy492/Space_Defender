#include "Models.h"

void MenuScene::Init() {
	Vector3 pos(10, 10, 0);
	Color color(1.0, 0.0, 0.0);
	Rectangle* rect = new Rectangle(pos, 100, 100, color);
	rect->Init();
	shapes.push_back(rect);
}

void MenuScene::ProcessInput(GLFWwindow* window) {

}

void MenuScene::Render() {
	for (auto& shape : shapes) {
		shape->Draw();
	}
}

MenuScene::~MenuScene() {
	for (auto& shape : shapes) {
		delete shape;
	}

	shapes.clear();
}