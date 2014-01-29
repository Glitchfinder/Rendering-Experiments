/*=================================                                       ----*\
 * GRAPHICS CLASS                                                             *
 * - This class operates as a self-creating, non-standard singleton used to   *
 *   manage the graphics and the game window.                                 *
\*----                                       =================================*/

#include "Graphics.h"

/** Defines the single Graphics instance **/
Graphics Graphics::Instance = *(new Graphics());

/** Graphics constructor **/
Graphics::Graphics() {
	Status = -1;
}

/** Initializes the class and creates the game window **/
int Graphics::initialize() {
	// Create the game window
	return Instance.createWindow();
}

/** External access to the draw function **/
void Graphics::update() {
	Instance.draw();
}

/** Access to the GLFW window for use elsewhere **/
GLFWwindow* Graphics::getWindow() {
	return Instance.Window;
}

/** Creates the game window and initializes OpenGL **/
int Graphics::createWindow() {
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		Status = -1;
		return Status;
	}

	// Set up an OpenGL window
	glfwWindowHint(GLFW_SAMPLES, 16);              // 16x AA
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);      // Not client resizable
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// We don't want the deprecated OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Set the window name
	const char* name = "Experiment 01 - A Window";
	// Create the window with OpenGL
	Window = glfwCreateWindow(640, 480, name, NULL, NULL);
	// Check to see if the window opened successfully
	if (!Window) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		Status = -1;
		return Status;
	}

	// Set the context as current
	glfwMakeContextCurrent(Window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		Status = -1;
		return Status;
	}

	// Return OK
	Status = 0;
	return Status;
}

/** Updates the game screen **/
void Graphics::draw() {
	// We're not drawing anything yet

	// Swap buffers
	glfwSwapBuffers(Window);
}
