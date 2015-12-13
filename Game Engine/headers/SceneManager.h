#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "GameConstants.h"
#include "Enemy.h"
#include "Player.h"

class Enemy;
class Player;

class SceneManager
{
public:

	// Getters
	std::vector<GameObject*> SceneManager::getGameObjects() { return gameObjects; }
	GameObject* SceneManager::getMainCamera() { return goMainCamera; }
	GameObject* SceneManager::getMainLight() { return goMainLight; }

	void cleanUp();
	void initScene();
	void updateGameObjects();

private:

	BasicCameraController *basicCameraController;

	std::vector<GameObject*> gameObjects;
	std::vector<Enemy*> enemies;
	Player *player;
	GameObject * goMainCamera;
	GameObject * goMainLight;
	GameObject * skyBox = NULL;

};
#endif
