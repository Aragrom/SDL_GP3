#ifndef RENDERER_H
#define RENDERER_H

#include "GameConstants.h"

class Renderer
{
public:

	void render(OpenGLLoader *openGLLoader,
		std::vector<GameObject*> gameObjects,
		PostProcessing postProcessor,
		GameObject *goMainCamera,
		GameObject *goMainLight);

	void renderGameObject(GameObject * pObject,
		GameObject *mainCamera,
		GameObject *mainLight);

	void renderSphere();

	void renderSkyBox(OpenGLLoader *openGLLoader,
		GameObject *goSkyBox,
		GameObject *goMainCamera);

private:
	
};
#endif
