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

/** Initializes the render test **/
void Graphics::initRenderTest() {
	// Load some shaders
	Shaders::loadShader("SimpleVertexShader.vshader", GL_VERTEX_SHADER);
	Shaders::loadShader("SimpleFragmentShader.fshader", GL_FRAGMENT_SHADER);
	Instance.ProgramID = Shaders::createProgram();

	// Set up an array of vectors
	const GLfloat vbData[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	// Generate one buffer and bind it
	glGenBuffers(1, &Instance.VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, Instance.VertexBuffer);
	// Toss the vertices and buffer at OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbData), vbData, GL_STATIC_DRAW);
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
	const char* name = "Experiment 02 - A Triangle";
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

	initOpenGL();

	// Return OK
	Status = 0;
	return Status;
}

/** Sets up OpenGL for the game **/
void Graphics::initOpenGL() {
	// Set up the Vertex Array Object
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Set the OpenGL clear color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

/** Updates the game screen **/
void Graphics::draw() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	glUseProgram(ProgramID);

	// First attribute: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(
		0,        // attribute 0
		3,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0,        // stride
		(void*) 0 // array buffer offset
	);

	// Draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); //Starting from vertex 0, 3 total
	glDisableVertexAttribArray(0);

	// Swap buffers
	glfwSwapBuffers(Window);
}
