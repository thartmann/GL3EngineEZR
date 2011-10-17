#include "Texture.h"

Texture::Texture()
{
	m_textureUnit = 0;
}

Texture::Texture(GLuint texID, GLuint texUnit, GLuint texSampler)
{
	m_textureUnit = texUnit;
	m_TexData.texID = texID;
	m_texSampler = texSampler;
}

Texture::~Texture()
{
	if(m_TexData.imageData != NULL)											// If imagedata has data in it
		{
			free(m_TexData.imageData);										// Delete data from memory
		}
}

bool Texture::LoadTGA(std::string filename)
{
	FILE * fTGA;															// File pointer to texture file
	fopen_s(&fTGA,filename.c_str(), "rb");									// Open file for reading

	if(fTGA == NULL)														// If it didn't open....
	{
		std::cout<< "Could not open texture file: " << filename <<std::endl;// Display an error message
		return false;														// Exit function
	}

	if(fread(&tgaheader, sizeof(Texture::TGAHeader), 1, fTGA) == 0)			// Attempt to read 12 byte header from file
	{
		std::cout<< "Could not read file header: " << filename <<std::endl;	// If it fails, display an error message
		if(fTGA != NULL)													// Check to see if file is still open
		{
			fclose(fTGA);													// If it is, close it
		}
		return false;														// Exit function
	}

	GLubyte uTGAcompare[12] = {0,0,2,0,0,0,0,0,0,0,0,0};					// Uncompressed TGA Header
	GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};					// Compressed TGA Header

	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)				// See if header matches the predefined header of
	{																		// an Uncompressed TGA image
		LoadUncompressedTGA(filename.c_str(), fTGA);								// If so, jump to Uncompressed TGA loading code
	}
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)		// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		LoadCompressedTGA(filename.c_str(), fTGA);									// If so, jump to Compressed TGA loading code
	}
	else																	// If header matches neither type
	{
		std::cout<< "TGA file be type 2 or type 10: " << filename <<std::endl;	// Display an error
		fclose(fTGA);
		return false;														// Exit function
	}
	return true;															// All went well, continue on
}

GLuint Texture::getID()
{
	return m_TexData.texID;
}

GLuint Texture::getUnit()
{
	return m_textureUnit;
}

GLuint Texture::getSampler()
{
	return m_texSampler;
}

bool Texture::LoadUncompressedTGA(const char * filename, FILE * fTGA)				// Load an uncompressed TGA (note, much of this code is based on NeHe's
{																			// TGA Loading code nehe.gamedev.net)
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Read TGA header
	{
		std::cout<< "Could not read info header"<< filename <<std::endl;	// Display error
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failular
	}

	m_TexData.width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	m_TexData.height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	m_TexData.bpp	= tga.header[4];										// Determine the bits per pixel
	tga.Width		= m_TexData.width;										// Copy width into local structure
	tga.Height		= m_TexData.height;										// Copy height into local structure
	tga.Bpp			= m_TexData.bpp;											// Copy BPP into local structure

	if((m_TexData.width <= 0) || (m_TexData.height <= 0) || ((m_TexData.bpp != 24) && (m_TexData.bpp !=32)))	// Make sure all information is valid
	{
		std::cout<< "Invalid texture information"<< filename <<std::endl;	// Display Error
		if(fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return false;														// Return failed
	}

	if(m_TexData.bpp == 24)													// If the BPP of the image is 24...
		m_TexData.type	= GL_RGB;											// Set Image type to GL_RGB
	else																	// Else if its 32 BPP
		m_TexData.type	= GL_RGBA;											// Set image type to GL_RGBA

	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	m_TexData.imageData	= (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

	if(m_TexData.imageData == NULL)											// If no space was allocated
	{
		std::cout<< "Could not allocate memory for image"<< filename <<std::endl;	// Display Error
		fclose(fTGA);														// Close the file
		return false;														// Return failed
	}

	if(fread(m_TexData.imageData, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
	{
		std::cout<< "Could not read image data"<< filename <<std::endl;		// Display Error
		if(m_TexData.imageData != NULL)										// If imagedata has data in it
		{
			free(m_TexData.imageData);										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

//	 Byte Swapping Optimized By Steve Thomas
	for(GLuint cswap = 0; cswap < (GLuint)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		m_TexData.imageData[cswap] ^= m_TexData.imageData[cswap+2] ^=
		m_TexData.imageData[cswap] ^= m_TexData.imageData[cswap+2];
	}

	fclose(fTGA);															// Close file
	CreateGLtexture();
	return true;															// Return success
}

bool Texture::LoadCompressedTGA(const char * filename, FILE * fTGA)				// Load COMPRESSED TGAs
{

    std::cout << "Reading compressed TGA" << std::endl;
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Attempt to read header
	{
		if(fTGA != NULL)													// If file is open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failed
	}

	m_TexData.width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	m_TexData.height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	m_TexData.bpp	= tga.header[4];										// Determine Bits Per Pixel
	tga.Width		= m_TexData.width;										// Copy width to local structure
	tga.Height		= m_TexData.height;										// Copy height to local structure
	tga.Bpp			= m_TexData.bpp;										// Copy bpp to local structure

	if((m_TexData.width <= 0) || (m_TexData.height <= 0) || ((m_TexData.bpp != 24) && (m_TexData.bpp !=32)))	//Make sure all texture info is ok
	{
		if(fTGA != NULL)													// Check if file is open
		{
			fclose(fTGA);													// Ifit is, close it
		}
		return false;														// Return failed
	}

	if(m_TexData.bpp == 24)													// If the BPP of the image is 24...
		m_TexData.type	= GL_RGB;											// Set Image type to GL_RGB
	else																	// Else if its 32 BPP
		m_TexData.type	= GL_RGBA;											// Set image type to GL_RGBA

	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
	m_TexData.imageData	= (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

	if(m_TexData.imageData == NULL)											// If it wasnt allocated correctly..
	{
		std::cout<< "Could not allocate memory for image"<< filename <<std::endl;	// Display Error
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	GLuint pixelcount	= tga.Height * tga.Width;							// Nuber of pixels in the image
	GLuint currentpixel	= 0;												// Current pixel being read
	GLuint currentbyte	= 0;												// Current byte
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

	do
	{
		GLubyte chunkheader = 0;											// Storage for "chunk" header

		if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)				// Read in the 1 byte header
		{
			std::cout<< "Could not read RLE header"<< filename <<std::endl;	// Display Error
			if(fTGA != NULL)												// If file is open
			{
				fclose(fTGA);												// Close file
			}
			if(m_TexData.imageData != NULL)									// If there is stored image data
			{
				free(m_TexData.imageData);									// Delete image data
			}
			return false;													// Return failed
		}

		if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
		{																	// that follow the header
			chunkheader++;													// add 1 to get number of following color values
			for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
			{
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel) // Try to read 1 pixel
				{
					std::cout<< "Could not read image data"<< filename <<std::endl;		// IF we cant, display an error

					if(fTGA != NULL)													// See if file is open
					{
						fclose(fTGA);													// If so, close file
					}

					if(colorbuffer != NULL)												// See if colorbuffer has data in it
					{
						free(colorbuffer);												// If so, delete it
					}

					if(m_TexData.imageData != NULL)										// See if there is stored Image data
					{
						free(m_TexData.imageData);										// If so, delete it too
					}

					return false;														// Return failed
				}
																						// write to memory
				m_TexData.imageData[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process
				m_TexData.imageData[currentbyte + 1	] = colorbuffer[1];
				m_TexData.imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
				{
					m_TexData.imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
				currentpixel++;															// Increase current pixel by 1

				if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
				{
					std::cout<< "Too many pixels read"<< filename <<std::endl;			// if there is too many... Display an error!

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(m_TexData.imageData != NULL)										// If there is Image data
					{
						free(m_TexData.imageData);										// delete it
					}

					return false;														// Return failed
				}
			}
		}
		else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
		{
			chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
			if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)		// Attempt to read following color values
			{
				std::cout<< "Could not read from file"<< filename <<std::endl;			// If attempt fails.. Display error (again)

				if(fTGA != NULL)														// If thereis a file open
				{
					fclose(fTGA);														// Close it
				}

				if(colorbuffer != NULL)													// If there is data in the colorbuffer
				{
					free(colorbuffer);													// delete it
				}

				if(m_TexData.imageData != NULL)											// If thereis image data
				{
					free(m_TexData.imageData);											// delete it
				}

				return false;															// return failed
			}

			for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated
			{																			// by the header
				m_TexData.imageData[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
				m_TexData.imageData[currentbyte + 1	] = colorbuffer[1];
				m_TexData.imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// If TGA images is 32 bpp
				{
					m_TexData.imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
				currentpixel++;															// Increase pixel count by 1

				if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
				{
					std::cout<< "Too many pixels read"<< filename <<std::endl;			// if there is too many... Display an error!

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(m_TexData.imageData != NULL)										// If there is Image data
					{
						free(m_TexData.imageData);										// delete it
					}

					return false;														// Return failed
				}
			}
		}
	}

	while(currentpixel < pixelcount);													// Loop while there are still pixels left
	fclose(fTGA);																		// Close the file
	CreateGLtexture();
	return true;																		// return success
}

bool Texture::CreateGLtexture()
{
	// Create one OpenGL texture
	glGenTextures(1, &m_TexData.texID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_TexData.texID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, m_TexData.type, m_TexData.width, m_TexData.height, 0, m_TexData.type, GL_UNSIGNED_BYTE, m_TexData.imageData);

	// Nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenSamplers(1, &m_texSampler);
	glSamplerParameteri(m_texSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(m_texSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(m_texSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_texSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return 0;
}
