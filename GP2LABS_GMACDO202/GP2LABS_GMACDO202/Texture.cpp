#include "Texture.h"

using namespace std;

/* Using SDL Image to load texture from file */
GLuint loadTextureFromFile(const string& filename)
{
	GLuint textureID = 0;
	SDL_Surface *imageSurface = IMG_Load(filename.c_str());

	if (!imageSurface){
		std::cout << "Can' Load image " << filename << " " << IMG_GetError();
		return textureID;
	}

	textureID = convertSDLSurfaceToGLTexture(imageSurface);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

GLuint loadTextureFromFont(const string& strFontFilename, int iPointSize, const string& strText)
{
	GLuint textureID = 0;

	#pragma region Load font using TTF_OpenFont
	TTF_Font *TTF_font = TTF_OpenFont(strFontFilename.c_str(), iPointSize);
	if (!TTF_font)
	{
		cout << "Unable to load font " << strFontFilename << " " << TTF_GetError();
		return textureID;
	}
	#pragma endregion

	/* create surface for function to render to
	http://sdl.beuc.net/sdl.wiki/SDL_ttf_Functions_Render
	   • Takes in a pointer to a loaded TTF_Font.
	   • string to be rendered.
	   • */
	SDL_Surface	*sdl_surfaceText = TTF_RenderText_Blended(TTF_font,
		strText.c_str(),
		{ 255, 255, 255 });

	textureID = convertSDLSurfaceToGLTexture(sdl_surfaceText);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Release memory allocated
	TTF_CloseFont(TTF_font);

	return textureID;
}

GLuint convertSDLSurfaceToGLTexture(SDL_Surface *sdl_surface)
{
	GLuint textureID = 0;
	GLint nOfColors = sdl_surface->format->BytesPerPixel;
	GLenum textureFormat = GL_RGB;
	GLenum internalFormat = GL_RGB8;
	if (nOfColors == 4) // contains an alpha channel
	{
		if (sdl_surface->format->Rmask == 0x000000ff)
		{
			textureFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else
		{
			textureFormat = GL_BGRA;
			internalFormat = GL_RGBA8;
		}
	}
	else if (nOfColors == 3) // no alpha channel
	{
		if (sdl_surface->format->Rmask == 0x000000ff)
		{
			textureFormat = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else
		{
			textureFormat = GL_BGR;
			internalFormat = GL_RGB8;
		}
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, sdl_surface->w, sdl_surface->h, 0,
	textureFormat, GL_UNSIGNED_BYTE, sdl_surface->pixels);
	SDL_FreeSurface(sdl_surface);

	return textureID;
}