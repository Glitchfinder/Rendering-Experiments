#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

/*=================================                                       ----*\
 * SHADERS CLASS                                                              *
 * - This static class operates as a factory to load, compile, and install    *
 *   shaders for use in rendering.                                            *
\*----                                       =================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <GL/glew.h> // GLEW must be included before gl.h or glfw.h
#include <GLFW/glfw3.h>

class Shaders {
	public:
		/** Loads, compiles, and tests a shader **/
		static bool   loadShader(const char* path, GLenum type);

		/** Creates a shader program **/
		static GLuint createProgram();
	protected:
	private:
		/** Prevent instantiation of the class **/
		Shaders() {};                              // No constructing
		Shaders(const Shaders& source);            // No copying
		Shaders& operator=(const Shaders& source); // No assignment

		/** Internal variable for shader processing **/
		static std::vector<GLuint> shaders;
};

#endif // SHADERS_H_INCLUDED
