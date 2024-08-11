
#include "Models.h"

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	Config& config = Config::GetInstance();
	config.SetScreenWidth(width);
	config.SetScreenHeight(height);
	glViewport(0, 0, width, height);
}


int main(int argc, char* argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Defender", NULL, NULL);

	if (!window) {
		std::cout << "GLFW failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	HWND windowHandle = glfwGetWin32Window(window);
	SetWindowLongPtr(windowHandle, GWL_STYLE, GetWindowLongPtrA(windowHandle, GWL_STYLE) & ~(WS_MAXIMIZEBOX | WS_THICKFRAME));

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD Failed to initalize" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	Config& config = Config::GetInstance();
	config.SetScreenWidth(SCREEN_WIDTH);
	config.SetScreenHeight(SCREEN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// init game objects
	SceneManager sceneManager;

	while (!glfwWindowShouldClose(window)) {
		sceneManager.currentActiveScene->ProcessInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		sceneManager.currentActiveScene->Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}