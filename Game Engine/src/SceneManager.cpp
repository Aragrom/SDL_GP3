#include "GameConstants.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

std::vector<GameObject*> SceneManager::getGameObjects() { return gameObjects; }
GameObject* SceneManager::getMainCamera() { return goMainCamera; }
GameObject* SceneManager::getMainLight() { return goMainLight; }

void SceneManager::updateGameObjects() 
{
	if (skyBox) skyBox->update();

	//Check for Player colliding with GameObjects colliders
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{

		vec3 v3PlayerPos = goMainCamera->getTransform()->getPosition();
		bool bPlayerCollision;

		std::string strColliderType = std::string("");
		//Store collider type
		if((*iter)->getCollider() != NULL) strColliderType = (*iter)->getCollider()->getColliderType();
		bool bResult;

		//if the collider type is BOX COLLIDER
		if (strColliderType == "BoxCollider")
		{
			BoxCollider *boxCollider = static_cast<BoxCollider*>((*iter)->getCollider());
			bResult = boxCollider->withinColliderCheck(v3PlayerPos);

			if (bResult)
			{
				std::cout << "Player collided with Box Collider on " << boxCollider->getParent()->getName() << " at position (" 
					<< v3PlayerPos.x << ","
					<< v3PlayerPos.y << ","
					<< v3PlayerPos.z << ")"
					<< std::endl;

				//Handle Collision (Current Primitive method)
				int iDir = basicCameraController->getDir();
				if (iDir > 0) basicCameraController->moveCamera("backward");
				else basicCameraController->moveCamera("forward");
			}
		}

		//if the collider type is SPHERE COLLIDER
		if (strColliderType == "SphereCollider")
		{
			SphereCollider *sphereCollider = static_cast<SphereCollider*>((*iter)->getCollider());
			bResult = sphereCollider->withinColliderCheck(v3PlayerPos);

			if (bResult)
			{
				std::cout << "Player collided with Sphere Collider on " << sphereCollider->getParent()->getName() << " at position ("
					<< v3PlayerPos.x << ","
					<< v3PlayerPos.y << ","
					<< v3PlayerPos.z << ")"
					<< std::endl;

				//Handle Collision (Current Primitive method)
				int iDir = basicCameraController->getDir();
				if (iDir > 0) basicCameraController->moveCamera("backward");
				else basicCameraController->moveCamera("forward");
			}
		}
	}

	//Check gameobjects colliding with one and other
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		//Store collider type
		/*std::string strColliderType = (*iter)->getCollider()->getColliderType();
		bool bResult;

		//if the collider type is box collider
		if (strColliderType == "BoxCollider") 
		{
			//Store original collider
			BoxCollider *boxCollider = static_cast<BoxCollider*>((*iter)->getCollider());

			//For all gameobjects compare to original collider
			for (auto iterCol = gameObjects.begin(); iter != gameObjects.end(); iterCol++)
			{
				//Collider to check against
				BoxCollider *otherBoxCollider = static_cast<BoxCollider*>((*iterCol)->getCollider());
				std::string strTestingType = (*iterCol)->getCollider()->getColliderType();

				if (strTestingType == "BoxCollider")
				{
					otherBoxCollider->withinColliderCheck(boxCollider);

					//if(collision detected) 
						//getVelocity and handle collision
							// handle the original object and the one it collides with
				}
			}
		}*/

		(*iter)->update();
	}
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string modelPath = ASSET_PATH + MODEL_PATH + "armoredrecon.fbx";

	//string holding address for texture
	std::string texturePath = ASSET_PATH + TEXTURE_PATH + "armoredrecon_diff.png";

	//string holding address for height map texture & specular map texture
	std::string heightMap = ASSET_PATH + TEXTURE_PATH + "armoredrecon_Height.png";
	std::string specularMap = ASSET_PATH + TEXTURE_PATH + "armoredrecon_spec.png";

	GameObject * armordrecon = loadFBXFromFile(modelPath);
	armordrecon->gameObject = armordrecon; //link with self
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

	armordrecon->getTransform()->setPosition(-0.0f, -1.0f, 0.0f);
	armordrecon->setLight(light);
	gameObjects.push_back(armordrecon);

	//Create BoxColliders
	BoxCollider *boxCollider;
	boxCollider = new BoxCollider(5.3f, 5.2f, 2.5f);
	boxCollider->setParent(armordrecon);
	boxCollider->createColliderVertices();
	armordrecon->setCollider(static_cast<Collider*>(boxCollider));
	basicCameraController->addGameObjectToTargets(armordrecon);
	 //##################################

	//Add BoxCollider to collider to detect for camera collision
	//colliders.push_back(collider);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	GameObject *goShape = new GameObject();
	goShape->gameObject = goShape; //Link with self
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

	newMat->setDiffuseColour(1.0f, 0.0f, 1.0f, 1.0f);
	newMat->setAmbientColour(1.0f, 0.0f, 1.0f, 1.0f);

	newMat->loadShader(vsPath, fsPath);

	goShape->setMaterial(newMat);

	SphereCollider *sphereCollider = new SphereCollider(5.0f);
	goShape->setCollider(static_cast<Collider*>(sphereCollider));

	//gameObjects.push_back(goShape); //#################################################

	basicCameraController->addGameObjectToTargets(goShape);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*GameObject *goSphere = new GameObject();

	Transform *tSphere = new Transform();
	tSphere->setPosition(0.0f,5.0f,0.0f);
	goSphere->setTransform(tSphere);

	goSphere->setName("Sphere");
	goSphere->setLight(light);

	Mesh *meshSphere = new Mesh();
	meshSphere->init();
	float radius = 100.0f;
	unsigned int rings = 10;
	unsigned int sectors = 10;
	Sphere *sphere = new Sphere(meshSphere, radius, rings, sectors);
	goSphere->setMesh(meshSphere);

	Material *matSphere = new Material();
	matSphere->init();
	vsPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
	fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
	matSphere->loadShader(vsPath, fsPath);
	goSphere->setMaterial(matSphere);*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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