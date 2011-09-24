#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include <stdio.h>

class Texture{
public:
	Texture();
	virtual ~Texture();
	bool LoadTGA(std::string name);
	GLuint getID();
	GLuint getUnit();

private:

	typedef	struct
	{
		GLubyte	* imageData;		// Image Data (Up To 32 Bits)
		GLuint	bpp;				// Image Color Depth In Bits Per Pixel
		GLuint	width;				// Image Width
		GLuint	height;				// Image Height
		GLuint	texID;				// Texture ID Used To Select A Texture
		GLuint	type;				// Image Type (GL_RGB, GL_RGBA)
	} data;

	typedef struct
	{
		GLubyte Header[12];			// TGA File Header
	} TGAHeader;


	typedef struct
	{
		GLubyte		header[6];		// First 6 Useful Bytes From The Header
		GLuint		bytesPerPixel;	// Holds Number Of Bytes Per Pixel Used In The TGA File
		GLuint		imageSize;		// Used To Store The Image Size When Setting Aside Ram
		GLuint		temp;			// Temporary Variable
		GLuint		type;
		GLuint		Height;			// Height of Image
		GLuint		Width;			// Width ofImage
		GLuint		Bpp;			// Bits Per Pixel
	} TGA;

	data m_TexData;

	GLuint m_textureUnit;
	TGAHeader tgaheader;
	TGA tga;

	bool LoadCompressedTGA(const char * filename, FILE * fTGA);
	bool LoadUncompressedTGA(const char * filename, FILE * fTGA);
	bool CreateGLtexture();
};

#endif //TEXTURE_H
