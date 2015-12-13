#include "GameConstants.h"

std::vector<GameObject*> SceneManager::getGameObjects() { return gameObjects; }
GameObject* SceneManager::getMainCamera() { return goMainCamera; }
GameObject* SceneManager::getMainLight() { return goMainLight; }

void SceneManager::updateGameObjects() 
{
	if (skyBox) skyBox->update();

	//alternative sytanx
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		(*iter)->update();
	}

	/*for (auto iter = colliders.begin(); iter != colliders.end(); iter++)
	{
		vec3 v3PlayerPos = goMainCamera->getTransform()->getPosition();
		bool bResult = (*iter)->checkForCollision(v3PlayerPos);

		if (bResult)
		{
			std::cout << "Player inside object at position ("
				<< v3PlayerPos.x << ","
				<< v3PlayerPos.y << ","
				<< v3PlayerPos.z << ")"
				<< std::endl;

			int iDir = basicCameraController->getDir();
			if (iDir > 0) basicCameraController->moveCamera("backward");
			else basicCameraController->moveCamera("forward");
		}
	}*/
}

void SceneManager::cleanUp()
{
	if (skyBox)
	{
		skyBox->destroy();
		delete skyBox;
		skyBox = NULL;
	}
	auto iter = gameObjects.begin();
	while (iter != gameObjects.end())
	{
		(*iter)->destroy();
		if ((*iter))
		{
			delete (*iter);
			(*iter) = NULL;
			iter = gameObjects.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	gameObjects.clear();
}

void SceneManager::initScene()
{
	//createSkyBox();

	std::string vsPath = ASSET_PATH + SHADER_PATH + "/passThroughVS.glsl";
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/colourFilterPostFS.glsl";

	//postProcessor.init(WINDOW_WIDTH, WINDOW_HEIGHT, vsPath, fsPath);

	goMainCamera = new GameObject();
	goMainCamera->setName("MainCamera");

	Transform *t = new Transform();
	t->setPosition(0.0f, 0.0f, 10.0f);

	goMainCamera->setTransform(t);

	basicCameraController = new BasicCameraController();
	basicCameraController->setAspectRatio((float)(iWINDOW_WIDTH / iWINDOW_HEIGHT));
	basicCameraController->setFOV(45.0f);
	basicCameraController->setNearClip(0.1f);
	basicCameraController->setFarClip(1000.0f);

	goMainCamera->setCamera(basicCameraController);

	gameObjects.push_back(goMainCamera);

	goMainLight = new GameObject();
	goMainLight->setName("MainLight");

	t = new Transform();
	t->setPosition(0.0f, 0.0f, 0.0f);
	goMainLight->setTransform(t);

	Light * light = new Light();
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	goMainLight->setLight(light);
	gameObjects.push_back(goMainLight);

	//add light as a target for camera controller to look at
	basicCameraController->addGameObjectToTargets(goMainLight);

	////////////////////////////////////////////////////////////////////////////////////////////
	// "armoredrecon" model : by Graham Alexander MacDonald
	// DIRECTIONAL LIGHT TEXTURE SHADER and HEIGHT MAPPING and SPECULAR HIGHLIGHT
	////////////////////////////////////////////////////////////////////////////////////////////

	std::string modelPath = ASSET_PATH + MODEL_PATH + "armoredrecon.fbx";

	//string holding address for texture
	std::string texturePath = ASSET_PATH + TEXTURE_PATH + "armoredrecon_diff.png";

	//string holding address for height map texture & specular map texture
	std::string heightMap = ASSET_PATH + TEXTURE_PATH + "armoredrecon_Height.png";
	std::string specularMap = ASSET_PATH + TEXTURE_PATH + "armoredrecon_spec.png";

	GameObject * armordrecon = loadFBXFromFile(modelPath);
	armordrecon->setName("armordrecon");
	for (int i = 0; i < armordrecon->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();

		material->loadDiffuseMap(texturePath);

		//load the heightmap texture
		material->loadHeightMap(heightMap);

		//load the specular texture
		material->loadSpecularMap(specularMap);

		material->setSpecularPower(50.0f);
		material->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

		std::string vsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureVS.glsl";
		std::string fsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureFS.glsl";
		material->loadShader(vsPath, fsPath);

		armordrecon->getChild(i)->setMaterial(material);
	}

	//add GameObject to vector of targets to look at in FPSController/BasicCameraController
	basicCameraController->addGameObjectToTargets(armordrecon);

	//Create BoxColliders
	Collider *collider = new Collider();
	collider->setLength(2.3f);
	collider->setWidth(5.2f);
	collider->setHeight(3.0f);

	//Set BoxCollider
	armordrecon->setCollider(collider);

	//BUG ERROR ON SETTING CENTRE BASED ON TRANSFORM
	armordrecon->getCollider()->setCentre(vec3(-0.0f, -1.0f, 0.0f));

	armordrecon->getTransform()->setPosition(-0.0f, -1.0f, 0.0f);
	armordrecon->setLight(light);
	gameObjects.push_back(armordrecon);

	//Add BoxCollider to collider to detect for camera collision
	//colliders.push_back(collider);

	GameObject *goShape = new GameObject();
	goShape->setName("Cube");
	goShape->setLight(light);
	goShape->setTransform(new Transform());

	Shape newShape = Shape();
	newShape.TEST(goShape);

	Material *newMat = new Material();
	newMat->init();

	//vsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureVS.glsl";
	//fsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureFS.glsl";

	//vsPath = ASSET_PATH + SHADER_PATH + "/ambientVS.glsl";
	//fsPath = ASSET_PATH + SHADER_PATH + "/ambientFS.glsl";

	vsPath = ASSET_PATH + SHADER_PATH + "/diffuseVS.glsl";
	fsPath = ASSET_PATH + SHADER_PATH + "/diffuseFS.glsl";

	newMat->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	newMat->setAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);

	newMat->loadShader(vsPath, fsPath);

	goShape->setMaterial(newMat);

	gameObjects.push_back(goShape);

	basicCameraController->addGameObjectToTargets(goShape);

	std::cout << "\nCONTROLS: Press F1 for 'help' (command list)" << std::endl;
	std::cout << "\nNAVIGATION:\n" << std::endl;
	std::cout << "[W]: Move camera FORWARD" << std::endl;
	std::cout << "[S]: Move camera BACKWARD" << std::endl;
	std::cout << "[A]: Rotate camera LEFT" << std::endl;
	std::cout << "[D]: Rotate camera RIGHT" << std::endl;
	std::cout << "[SPACE]: Move camera UP" << std::endl;
	std::cout << "[LEFT_SHIFT]: Move camera DOWN\n" << std::endl;
	std::cout << "OBJECT MANIPULATION\n" << std::endl;
	std::cout << "[CAPS_LOCK]: Target scene objects " << std::endl;
	std::cout << "[TAB]: Iterate through scene objects\n" << std::endl;
	std::cout << "...while locked onto an object" << std::endl;
	std::cout << "[Q]: Rotate positively round an object " << std::endl;
	std::cout << "[E]: Rotate negatively round an object\n" << std::endl;
	std::cout << "...while holding " << std::endl;
	std::cout << "[X]: round the x-axis " << std::endl;
	std::cout << "[Y]: round the y-axis " << std::endl;
	std::cout << "[Z]: round the z-axis\n" << std::endl;
	std::cout << "...To move objects" << std::endl;
	std::cout << "[UP_ARROW][DOWN_ARROW][LEFT_ARROW][RIGHT_ARROW]" << std::endl;

	////////////////////////////////////////////////////////////////////////////////////////////

	//Start Game Clock
	Timer::getTimer().start();
}