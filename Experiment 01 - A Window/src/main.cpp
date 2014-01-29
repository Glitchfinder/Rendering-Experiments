#include <stdio.h>
#include <stdlib.h>
#include <ctime>
// GLEW must be included before gl.h or glfw.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Create an OpenGL window
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x AA
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// We don't want the deprecated OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Set the window name
	const char* name = "Experiment 01 - A Window";
	// Create the window with OpenGL
	GLFWwindow*  window = glfwCreateWindow(640, 480, name, NULL, NULL);
	// Check to see if the window opened successfully
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}

	// Set the context as current
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		//return -1;
	}

	// Set up an actual game loop
	bool   running = true;
	time_t lastSync;
	time_t currentTime;
	double timeSinceLastRefresh;
	time(&lastSync);

	// Set up a frame counter
	double lastTime = glfwGetTime();
	int nbFrames    = 0;

	while (running) {
		// Measure frame speed
		double cTime = glfwGetTime();
		nbFrames++;

		if (cTime - lastTime >= 1.0) {
			fprintf(stdout, "%f ms/frames\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Check timing
		time(&currentTime);
		timeSinceLastRefresh = 1000.0 * difftime(currentTime, lastSync);

		// Update graphics if appropriate
		if (timeSinceLastRefresh >= 0.016) { // 16MS = 60FPS
			// We're not drawing anything yet

			// Swap buffers
			glfwSwapBuffers(window);

			// Update sync
			lastSync = currentTime;
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
