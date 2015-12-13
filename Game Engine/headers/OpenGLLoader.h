#ifndef OPENGLLOADER_H
#define OPENGLLOADER_H

#include "GameConstants.h"

class OpenGLLoader
{
public:

	void cleanUp();
	void checkForErrors();
	void initWindow(int width, int height, bool fullscreen);
	void initOpenGL();
	void setViewport(int width, int height);
	void handleEvents();

	SDL_Window* getWindow();
	SDL_GLContext getContext();
	bool isActive();

private:

	bool bIsActive = true;
	SDL_Window *window = NULL;
	SDL_GLContext glcontext = NULL;
	SDL_Event event;

};
#endif
