#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

/*=================================                                       ----*\
 * GRAPHICS CLASS                                                             *
 * - This class operates as a self-creating, non-standard singleton used to   *
 *   manage the graphics and the game window.                                 *
\*----                                       =================================*/

#include <stdio.h>
#include <stdlib.h>
#include "Shaders.h"
#include <GL/glew.h> // GLEW must be included before gl.h or glfw.h
#include <GLFW/glfw3.h>

class Graphics {
	public:
		/** Initializes the Graphics class and creates the window **/
		static int initialize();

		/** Updates the screen **/
		static void update();

		/** Access method to grab the GLFW Window for use elsewhere **/
		static GLFWwindow* getWindow();

		/** Initializes the render test **/
		static void initRenderTest();
	protected:
	private:
		/** The singleton instance **/
		static Graphics Instance;

		/** Internal variables for graphics processing **/
		GLFWwindow* Window;
		GLuint VertexArrayID, ProgramID, VertexBuffer;
		int Status;

		/** Private constructor to ensure only one instance exists **/
		Graphics();                                  // No constructing

		/** Internal functions used for creation and processing **/
		int  createWindow();
		void initOpenGL();
		void draw();
};

#endif // GRAPHICS_H_INCLUDED
