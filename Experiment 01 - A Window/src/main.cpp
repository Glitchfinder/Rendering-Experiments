#include "Graphics.h"
#include <ctime>

int main() {
	// Initialize and check Graphics
	if (Graphics::initialize() != 0)
		return -1;

	// Get the game window for future use
	GLFWwindow* window = Graphics::getWindow();
	// Ensure capture of the escape key
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set up an actual game loop
	bool   running    = true;
	double lastSync   = glfwGetTime();
	double targetRate = 16.0 / 1000.0;

	// Set up a frame counter
	double lastTime = glfwGetTime();
	int    frames   = 0;

	while (running) {
		// Measure frame speed
		double cTime = glfwGetTime();
		frames++;

		if (cTime - lastTime >= 1.0) {
			fprintf(stdout, "%f ms/frame\n", 1000.0/double(frames));
			frames = 0;
			lastTime += 1.0;
		}

		// Update graphics if appropriate
		if (cTime - lastSync >= targetRate) { //16MS = 60FPS
			Graphics::update();
			// Update sync
			lastSync = glfwGetTime();
		}

		// Poll events
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
			glfwWindowShouldClose(window))
		{
			running = false;
		}
	}
}
