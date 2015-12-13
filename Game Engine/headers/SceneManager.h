#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "GameConstants.h"
#include "Enemy.h"
#include "Player.h"
#include "Planet.h"

class Enemy;
class Player;
class Planet;

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
	void updatePlanets();
	void updatePlayer();

	void incrementBuildPosition();
	void rotateBuildDirection();
	void instantiateFloorSegment(vec3 v3InstantiatePosition);
	void createSkyBox();

	GameObject * skyBox = NULL;

private:

	BasicCameraController *basicCameraController;

	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> floorTiles;
	std::vector<Enemy*> enemies;
	std::vector<Planet*> planets;
	Player *player;
	GameObject * goMainCamera;
	GameObject * goMainLight;

	int iLevelSize;
	int iBuildDirection;
	vec3 v3BuildPosition;

	int iAmountOfStars;
};
#endif
