#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
// GLEW must be included before gl.h or glfw.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Cheap and easy forward declarations
void draw(GLuint, GLuint, GLuint, GLuint, glm::mat4);
GLint loadShaders(const char*, const char*);

int main() {
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Create an OpenGL window
	glfwWindowHint(GLFW_SAMPLES, 16); // 4x AA
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// We don't want the deprecated OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Set the window name
	const char* name = "Experiment 04 - A Cube";
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
		return -1;
	}

	// Ensure capture of the escape key
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set up the Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Set the OpenGL clear color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth handling
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Load some shaders
	const char* vShader = "Transform.vshader";
	const char* fShader = "Color.fshader";
	GLuint pid = loadShaders(vShader, fShader);

	// Set up an array of vectors
	static const GLfloat vbData[] = {
		-1.0f, -1.0f, -1.0f, // triangle : begin
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, // triangle : begin
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f, // triangle : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f, // triangle : begin
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f, // triangle : begin
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f, // triangle : begin
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f, // triangle : begin
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f, // triangle : begin
		-1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f, // triangle : begin
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f, // triangle : begin
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f, // triangle : begin
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f, // triangle : begin
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f
	};

	// Vertex buffer id
	GLuint vBuffer;
	// Generate one buffer and bind it
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	// Toss the vertices and buffer at OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbData), vbData, GL_STATIC_DRAW);

	// Seed random and get base color values
	srand(static_cast<unsigned>(time(0)));
	float posR = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float negR = 1.0f - posR;
	float posG = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float negG = 1.0f - posG;
	float posB = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float negB = 1.0f - posB;

	float modR = 0.0f;
	float modG = 0.0f;
	float modB = 0.0f;

	// Vertex colors
	static GLfloat cbData[12*3*3];
	// Generate vertex colors
	for (int v = 0; v < 12 * 3; v++) {
		if (v % 6 == 0) {
			modR = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			modG = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			modB = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		}

		cbData[3 * v    ] = (vbData[3 * v    ] > 0 ? posR : negR); // Red
		cbData[3 * v + 1] = (vbData[3 * v + 1] > 0 ? posG : negG); // Green
		cbData[3 * v + 2] = (vbData[3 * v + 2] > 0 ? posB : negB); // Blue

		cbData[3 * v    ] = (cbData[3 * v    ] + modR) / 2.0f; // Red
		cbData[3 * v + 1] = (cbData[3 * v + 1] + modG) / 2.0f; // Green
		cbData[3 * v + 2] = (cbData[3 * v + 2] + modB) / 2.0f; // Blue
	}

	// Color buffer id
	GLuint cBuffer;
	// Generate one buffer and bind it
	glGenBuffers(1, &cBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cBuffer);
	// Toss the vertices and buffer at OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(cbData), cbData, GL_STATIC_DRAW);

	// Get a handle for our mvp uniform
	// Only do this at initialization
	GLuint mid = glGetUniformLocation(pid, "MVP");

	// Make a projection matrix (FoV, aspect ratio, range-min, range-max)
	glm::mat4 proj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Make a camera matrix
	glm::mat4 view = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera location
		glm::vec3(0, 0, 0), // Camera aim location
		glm::vec3(0, 1, 0)  // Head is up (0, -1, 0 is upside-down)
	);

	// Make a model matrix
	glm::mat4 mod = glm::mat4(1.0f); // Necessary for each model
	// Our ModelViewProjection
	glm::mat4 mvp = proj * view * mod;

	// Set up an actual game loop
	bool   running  = true;
	double lastSync = glfwGetTime();

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

			// Seed random and get base color values
			srand(static_cast<unsigned>(rand()));
			float posR = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float negR = 1.0f - posR;
			float posG = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float negG = 1.0f - posG;
			float posB = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float negB = 1.0f - posB;

			float modR = 0.0f;
			float modG = 0.0f;
			float modB = 0.0f;

			// Vertex colors
			static GLfloat cbData[12*3*3];
			// Generate vertex colors
			for (int v = 0; v < 12 * 3; v++) {
				if (v % 6 == 0) {
					modR = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
					modG = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
					modB = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				}

				cbData[3 * v    ] = (vbData[3 * v    ] > 0 ? posR : negR); // Red
				cbData[3 * v + 1] = (vbData[3 * v + 1] > 0 ? posG : negG); // Green
				cbData[3 * v + 2] = (vbData[3 * v + 2] > 0 ? posB : negB); // Blue

				cbData[3 * v    ] = (cbData[3 * v    ] + modR) / 2.0f; // Red
				cbData[3 * v + 1] = (cbData[3 * v + 1] + modG) / 2.0f; // Green
				cbData[3 * v + 2] = (cbData[3 * v + 2] + modB) / 2.0f; // Blue
			}

			// Toss the vertices and buffer at OpenGL
			glBufferData(GL_ARRAY_BUFFER, sizeof(cbData), cbData, GL_STATIC_DRAW);
		}

		// Update graphics if appropriate
		if (cTime - lastSync >= 16.0 / 1000.0) { //16MS = 60FPS
			draw(pid, mid, vBuffer, cBuffer, mvp);

			// Swap buffers
			glfwSwapBuffers(window);

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

void draw(GLuint pid, GLuint mid, GLuint vBuff, GLuint cBuff, glm::mat4 mvp) {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Use our shader
	glUseProgram(pid);

	// Send the transformation to the shader for every model we render
	glUniformMatrix4fv(mid, 1, GL_FALSE, &mvp[0][0]);

	// First attribute: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vBuff);
	glVertexAttribPointer(
		0,        // attribute 0
		3,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0,        // stride
		(void*) 0 // array buffer offset
	);

	// Second attribute: vertices
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cBuff);
	glVertexAttribPointer(
		1,        // attribute 0
		3,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0,        // stride
		(void*) 0 // array buffer offset
	);

	// Draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 12*3); //Starting from vertex 0, 3 total
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
