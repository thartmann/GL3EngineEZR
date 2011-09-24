#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

/*!
 *\brief	common includes for LPV framework
 *
 *\author	Tobias Hartmann
 *\date		07-07-2011
 */

#ifdef UNIX
//OpenGL3+
///* If using gl3.h */
///* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

//glfw for windowing and user input
#include <GL/glfw.h>

#endif

#ifdef WIN
#include <GL/glew.h>
#include <gl/freeglut.h>
#endif

////glm is the openGL mathematics library
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

//stl includes
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define TS_LOCAL 0
#define TS_GLOBAL 1


inline bool CheckOpenGLError(std::string identifier)
{
	GLenum err(glGetError());

	std::string error;
	switch (err)
	{
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			error = "Unknown error";
			break;
	}

    if (err != GL_NO_ERROR)
    {
        std::cerr << "[ERROR] OpenGL threw error \"" << error << "\" at " << identifier << "!" << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}


#endif // COMMON_H_INCLUDED
