#include "MultipleRenderTarget.h"

MultipleRenderTarget::MultipleRenderTarget(void)
{
	mrtTexture = new GLuint[5];

	mrtTextureUnit = new GLuint[5];
	mrtTextureUnit[0] = 0;
	mrtTextureUnit[1] = 1;
	mrtTextureUnit[2] = 2;
	mrtTextureUnit[3] = 3;
	mrtTextureUnit[4] = 4;
}

MultipleRenderTarget::~MultipleRenderTarget(void)
{
	delete[] mrtTexture;
	delete[] mrtTextureUnit;
}

void MultipleRenderTarget::createMRT( int width, int height)
{
	//Generate Framebuffer
	glGenFramebuffers(1, &fbo );
	glBindFramebuffer(GL_FRAMEBUFFER, fbo );

	glGenTextures(5, mrtTexture);

	for( int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, mrtTexture[i] );

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// filtering is needed, but need to be checked !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindTexture( GL_TEXTURE_2D, mrtTexture[4] );
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	// filtering is needed, but need to be checked !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	//generate sampler for textures
	glGenSamplers(1, &mrtSampler);
	glSamplerParameteri(mrtSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(mrtSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(mrtSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(mrtSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Depthbuffer, need to be checked	
	glGenRenderbuffers(1, &dbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dbo);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mrtTexture[0], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, mrtTexture[1], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, mrtTexture[2], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, mrtTexture[3], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mrtTexture[4], 0);

	GLenum DrawBuffers[4] = 
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3
	};
	glDrawBuffers(4, DrawBuffers);

	CheckOpenGLError("createMRT()");
}

void MultipleRenderTarget::createFSQ()
{
	static const GLfloat g_quad_vertex_buffer_data[] = {
	    -1.0f, -1.0f, 0.0f,
	    1.0f, -1.0f, 0.0f,
	    -1.0f,  1.0f, 0.0f,
	    -1.0f,  1.0f, 0.0f,
	    1.0f, -1.0f, 0.0f,
	    1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

}

void MultipleRenderTarget::useMRT()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo );
	CheckOpenGLError("useMRT()");
}

GLuint MultipleRenderTarget::getTexture( int a )
{
	return mrtTexture[a];
}

GLuint MultipleRenderTarget::getTextureUnit( int a )
{
	return mrtTextureUnit[a];
}

GLuint MultipleRenderTarget::getSampler(int a)
{
	return mrtSampler;
}

GLuint MultipleRenderTarget::getFSQbuffer()
{
	return quad_vertexbuffer;
}
