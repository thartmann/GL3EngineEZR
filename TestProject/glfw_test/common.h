#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

/*!
 *\brief	common includes for framework
 *
 *\author	Tobias Hartmann
 *\date		07-07-2011
 */

//OpenGL3+
///* If using gl3.h */
///* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

//glfw for windowing and user input
#include <GL/glfw.h>

//assimp for model loading
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>
#include <assimp/DefaultLogger.h>
#include <assimp/LogStream.h>

//devIL for image handling
#include <IL/il.h>

//glm is the openGL mathematics library
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//stl includes
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <map>


typedef struct
{
	float position[4];
	float color[4];
} Vertex;


#endif // COMMON_H_INCLUDED
