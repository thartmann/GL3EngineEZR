#ifndef MULTIPLERENDERTARGET_H
#define MULTIPLERENDERTARGET_H

#include "common.h"

class MultipleRenderTarget
{

public: 

	MultipleRenderTarget(void);
	~MultipleRenderTarget(void);

	GLuint getTexture(int a);
	GLuint getTextureUnit(int a);
	GLuint getSampler(int a);

	GLuint getFSQbuffer();

	void createMRT( int width, int height );
	void createFSQ();
	void useMRT();

private:

//framebufferobject
	GLuint fbo;
//depthbufferobject
	GLuint dbo;
//texturearray		0: Color 1:vertexposition; 2:vertexnormals; 3:vertexambientvalue; 4:depthvalue
	GLuint *mrtTexture;
	GLuint *mrtTextureUnit;
	GLuint mrtSampler;

	GLuint quad_vertexbuffer;

//	Shader *ShaderMRT;

};

#endif