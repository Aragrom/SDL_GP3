#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "GameConstants.h"

class SceneManager
{
public:

	std::vector<GameObject*> getGameObjects();
	GameObject* getMainCamera();
	GameObject* getMainLight();
	void cleanUp();
	void initScene();
	void updateGameObjects();

private:

	BasicCameraController *basicCameraController;

	std::vector<GameObject*> gameObjects;
	GameObject * goMainCamera;
	GameObject * goMainLight;
	GameObject * skyBox = NULL;

};
#endif
