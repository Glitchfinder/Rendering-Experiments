#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
// GLEW must be included before gl.h or glfw.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Cheap and easy forward declarations
void draw(GLuint, GLuint);
GLint loadShaders(const char*, const char*);

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
	const char* name = "Experiment 02 - A Triangle";
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

	// Ensure capture of the escape key
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set up an actual game loop
	bool   running = true;
	time_t lastSync;
	time_t currentTime;
	double timeSinceLastRefresh;
	time(&lastSync);

	// Set up the Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Set the OpenGL clear color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Load some shaders
	const char* vShader = "SimpleVertexShader.vshader";
	const char* fShader = "SimpleFragmentShader.fshader";
	GLuint pid = loadShaders(vShader, fShader);

	// Set up an array of vectors
	static const GLfloat vbData[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	// Vertex buffer id
	GLuint vertexBuffer;
	// Generate one buffer and bind it
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// Toss the vertices and buffer at OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbData), vbData, GL_STATIC_DRAW);

	while (running) {
		// Check timing
		time(&currentTime);
		timeSinceLastRefresh = 1000.0 * difftime(currentTime, lastSync);

		// Update graphics if appropriate
		if (timeSinceLastRefresh >= 0.016) { // 16MS = 60FPS
			// We're not drawing anything yet
			draw(pid, vertexBuffer);

			// Swap buffers
			glfwSwapBuffers(window);
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

void draw(GLuint pid, GLuint vertexBuffer) {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	glUseProgram(pid);

	// First attribute: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
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
}

GLint loadShaders(const char* vFilePath, const char* fFilePath) {
	// Create the shaders
	GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Grab the vertex shader from file
	std::string vShaderCode;
	std::ifstream vShaderStream(vFilePath, std::ios::in);

	if (vShaderStream.is_open()) {
		std::string Line = "";

		while (getline(vShaderStream, Line))
			vShaderCode += "\n" + Line;

		vShaderStream.close();
	}

	// Grab the fragment shader from file
	std::string fShaderCode;
	std::ifstream fShaderStream(fFilePath, std::ios::in);

	if (fShaderStream.is_open()) {
		std::string Line = "";

		while (getline(fShaderStream, Line))
			fShaderCode += "\n" + Line;

		fShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infLogLen;

	// Compile the vertex shader
	printf("Compiling shader : %s\n", vFilePath);
	const char* vSrcPntr = vShaderCode.c_str();
	glShaderSource(vShaderID, 1, &vSrcPntr, NULL);
	glCompileShader(vShaderID);

	// Check the vertex shader
	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vShaderID, GL_INFO_LOG_LENGTH, &infLogLen);
	std::vector<char> vShaderErrMsg(infLogLen);
	glGetShaderInfoLog(vShaderID, infLogLen, NULL, &vShaderErrMsg[0]);
	fprintf(stdout, "%s\n", &vShaderErrMsg[0]);

	// Compile the fragment shader
	printf("Compiling shader : %s\n", fFilePath);
	const char* fSrcPntr = fShaderCode.c_str();
	glShaderSource(fShaderID, 1, &fSrcPntr, NULL);
	glCompileShader(fShaderID);

	// Check the vertex shader
	glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fShaderID, GL_INFO_LOG_LENGTH, &infLogLen);
	std::vector<char> fShaderErrMsg(infLogLen);
	glGetShaderInfoLog(fShaderID, infLogLen, NULL, &fShaderErrMsg[0]);
	fprintf(stdout, "%s\n", &fShaderErrMsg[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint pid = glCreateProgram();
	glAttachShader(pid, vShaderID);
	glAttachShader(pid, fShaderID);
	glLinkProgram(pid);

	// Check the program
	glGetProgramiv(pid, GL_LINK_STATUS, &result);
	glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &infLogLen);
	std::vector<char> pErrMsg(std::max(infLogLen, int(1)));
	glGetProgramInfoLog(pid, infLogLen, NULL, &pErrMsg[0]);
	fprintf(stdout, "%s\n", &pErrMsg[0]);

	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);

    return pid;
}
