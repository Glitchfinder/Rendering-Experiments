/*=================================                                       ----*\
 * SHADERS CLASS                                                              *
 * - This static class operates as a factory to load, compile, and install    *
 *   shaders for use in rendering.                                            *
\*----                                       =================================*/

#include "Shaders.h"

/** Tracks shaders that have been defined but not used **/
std::vector<GLuint> Shaders::shaders;

/** Loads, compiles, and tests a shader **/
bool Shaders::loadShader(const char* path, GLenum type) {
	// Create the shader
	GLuint shaderID = glCreateShader(type);

	// Initialize the shader stream
	std::string   shaderCode;
	std::ifstream shaderStream(path, std::ios::in);

	// Read the stream to code
	if (shaderStream.is_open()) {
		std::string line = "";

		while (getline(shaderStream, line))
			shaderCode += "\n" + line;

		shaderStream.close();
	}

	GLint result = GL_FALSE;
	int   logLength;

	// Compile the shader
	printf("Compiling shader : %s\n", path);
	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	// Check the shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> errMsg(logLength);
	glGetShaderInfoLog(shaderID, logLength, NULL, &errMsg[0]);
	if (errMsg.size() > 0)
		fprintf(stdout, "%s\n", &errMsg[0]);

	shaders.push_back(shaderID);

	return (result == GL_TRUE);
}

/** Creates a shader program **/
GLuint Shaders::createProgram() {
	// Create the program
	fprintf(stdout, "Linking shader program\n");
	GLuint programID = glCreateProgram();

	// Attach the current shader list
	std::vector<GLuint>::iterator e = shaders.begin();
	for(; e != shaders.end(); ++e) {
		glAttachShader(programID, *e);
	}

	// Link the shaders
	glLinkProgram(programID);

	// Check the program
	GLint result = GL_FALSE;
	int   logLength;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		std::vector<char> errMsg(logLength);
		glGetProgramInfoLog(programID, logLength, NULL, &errMsg[0]);
		fprintf(stdout, "%s\n", &errMsg[0]);
	}

	// Delete the attached shaders
	e = shaders.begin();
	for(; e != shaders.end(); ++e) {
		glDeleteShader(*e);
	}

	shaders.clear();
	return programID;
}
