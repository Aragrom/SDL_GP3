/*
=================
cTexture.h
- Header file for class definition - SPECIFICATION
- Header file for the cD3DXTexture class
=================
*/

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "GameConstants.h"
#include "Texture.h"

class Texture
{
private:
	GLuint GLTextureID;
	GLuint aTextureInfo;
	GLsizei textureWidth;
	GLsizei textureHeight;
	ILuint ilTextureID;
	ILubyte *txData;

public:
	Texture();
	Texture(LPCSTR theFilename);
	~Texture();
	bool createTexture(LPCSTR theFilename);		// create the texture for use.
	GLuint getTexture();						// return the texture.
	GLsizei getTWidth();						// Return width of texture;
	GLsizei getTHeight();						// Return height of texture;
};
#endif
