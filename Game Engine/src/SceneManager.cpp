#include "SceneManager.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

void SceneManager::updatePlayer()
{
	if(player->getIsFalling()) player->update();
}

void SceneManager::updatePlanets() 
{
	for (auto iter = planets.begin(); iter != planets.end(); iter++)
	{
		(*iter)->update();
	}
}

void SceneManager::updateGameObjects() 
{
	if (skyBox) skyBox->update();
	//Check for Player colliding with GameObjects colliders
	
	vec3 v3PlayerPos = player->getGameObject()->getTransform()->getPosition();
	v3PlayerPos = v3PlayerPos + vec3(0.0f, -0.5f, 0.0f);		//Adjust position to check just below player
	bool bOnGround = false;

	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		std::string strColliderType = std::string("");
		//Store collider type
		if ((*iter)->getCollider() != NULL) strColliderType = (*iter)->getCollider()->getColliderType();

		//if the collider type is BOX COLLIDER
		if (strColliderType == "BoxCollider")
		{
			BoxCollider *boxCollider = static_cast<BoxCollider*>((*iter)->getCollider());
			if(boxCollider->withinColliderCheck(v3PlayerPos)) bOnGround = true;
		}
	}

	player->setIsFalling(!bOnGround);
	BasicCameraController *basicCameraController = static_cast<BasicCameraController*>(player->getGameObject()->getCamera());
	basicCameraController->setCanMove(bOnGround);
	player->update(); // UPDATE PLAYER

	//Check for Enemys functionality
	for (auto iter = enemies.begin(); iter != enemies.end(); iter++)
	{
		(*iter)->update();
	}

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
	enemies.clear();
}

void SceneManager::initScene()
{
	//==============================================================================
	// !REQUIRED! GameObjects and Components
	//==============================================================================

	//Initialising MAIN CAMERA CONTROLLER
	basicCameraController = new BasicCameraController();								// Instantiate Basic Camera Controller (Camera Component)
	basicCameraController->setAspectRatio((float)(iWINDOW_WIDTH / iWINDOW_HEIGHT));		// Define aspect ratio
	basicCameraController->setFOV(45.0f);												// Set Field of View
	basicCameraController->setNearClip(0.1f);											// Set how close objects can be before clipped and not rendered
	basicCameraController->setFarClip(1000.0f);											// Set how far objects can be before clipped and not rendered

	//Initialise GAMEOBJECT that will HOLD BASIC CAMERA CONTROLLER (CAMERA COMPONENT)
	goMainCamera = new GameObject();													// Instantiate Camera GameObject
	goMainCamera->setName("MainCamera");												// Set name property in gameobject for main camera
	Transform *t = new Transform();														// Define new Transform for camera
	t->setPosition(V3_START_POSITION.x, V3_START_POSITION.y, V3_START_POSITION.z);													// Set Transform position
	goMainCamera->setTransform(t);														// Set Transform components onto the GameObject
	goMainCamera->setCamera(basicCameraController);										// Set GameObjects camera components to the basic camera controller

	//Initialise MAIN LIGHT
	Light * light = new Light();														// Instantiate main light (Light Component)
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);									// Set Diffuse Colour

	//Initialise GAMEOBJECT that will HOLD MAIN LIGHT (LIGHT COMPONENT)
	goMainLight = new GameObject();														// Instantiate Light GameObject
	goMainLight->setName("MainLight");													// Set name property in gameobject for main light
	t = new Transform();																// Define new Transform for light
	t->setPosition(0.0f, 0.0f, 0.0f);													// Set Transform position
	goMainLight->setTransform(t);														// Set Transform component onto the GameObject
	goMainLight->setLight(light);														// Set GameObjects light component to the main light
	//goMainCamera->setLight(light);													// Set Main Camera's GameObject to as main light's light component

	//Add GameObjects to the vector for render, updating etc.
	gameObjects.push_back(goMainCamera);
	gameObjects.push_back(goMainLight);

	createSkyBox();

	//==============================================================================
	// Define and link PLAYER components
	//==============================================================================

	player = new Player();
	player->setGameObject(goMainCamera);

	//==============================================================================
	// Scene Objects
	//==============================================================================

	// String address values
	std::string vsPath;				// Holds string for Vertex Shader address
	std::string fsPath;				// Holds string for Fragment Shader address

	std::string modelPath;			// Holds string for model path address
	std::string texturePath;		// Holds string for Texture Map path address
	std::string heightMap;			// Holds string for Height Map path address
	std::string specularMap;		// Holds string for Specular Map path address

	//==============================================================================
	// Oil Barrel Model
	//==============================================================================

	modelPath = ASSET_PATH + MODEL_PATH + "barrel_1.fbx";								// Set string for model address
	texturePath = ASSET_PATH + TEXTURE_PATH + "barrel_1_diffuse.png";					// Set string for texture Map address
	heightMap = ASSET_PATH + TEXTURE_PATH + "barrel_1_height.png";						// Set string for height map address
	specularMap = ASSET_PATH + TEXTURE_PATH + "barrel_1_specular.png";					// Set string for specular map address

	// Initialise GAMEOBJECT that will HOLD THE OIL BARREL
	GameObject * oilBarrel = loadFBXFromFile(modelPath, vec3(1.0f, 1.0f, 1.0f));		// Load model OIL BARREL
	oilBarrel->gameObject = oilBarrel;													// link with self
	oilBarrel->setName("Oil Barrel");													// Set name property in oilBarrel's GameObject
	t = new Transform();																// Define new Transform for oilBarrel's GameObject
	t->setPosition(2.0f, -1.0f, 1.0f);													// Set Transform position
	t->setRotation(270.0f, 0.0f, 0.0f);													// Barrel needs to be flipped to be up right
	oilBarrel->setTransform(t);															// Set Transform component onto the GameObject
	//goMainLight->setLight(light);														// Set GameObjects light component to the main light
	vsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureVS.glsl";				// Set string for Vertex Shader
	fsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureFS.glsl";				// Set string for Fragment Shader

	//For all child objects attach a new MATERIAL LOAD VERTEX and FRAGMENT SHADER
	for (int i = 0; i < oilBarrel->getChildCount(); i++)
	{
		// Set MATERIAL Properties
		Material * material = new Material();											// Instantiate new material
		material->init();																// Initiate material
		material->loadDiffuseMap(texturePath);											// Load Diffuse Texture
		material->loadHeightMap(heightMap);												// Load Height Map
		material->loadSpecularMap(specularMap);											// Load Specular Map
		material->setSpecularPower(50.0f);												// Set Specular power for material
		material->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);							// Set Specular colour for material
		material->loadShader(vsPath, fsPath);											// Load Vertex and Fragment Shader onto the material

		oilBarrel->getChild(i)->setMaterial(material);									// Set child material
	}

	// Initialise BOX COLLIDER for ArmorDrecon
	BoxCollider *boxCollider;															// Declare memory for Box Collider							
	boxCollider = new BoxCollider(2.0f, 2.0f, 2.0f);									// Instantiate Box Collider
	boxCollider->setParent(oilBarrel);													// Set Box Collider parent as the oilBarrel's GameObject
	boxCollider->createColliderVertices();												// Initialise the box collider properties
	oilBarrel->setCollider(static_cast<Collider*>(boxCollider));						// Set the Collider component on the oilBarrel as a cast box collider
	basicCameraController->addGameObjectToTargets(oilBarrel);							// Add the GameObjects the cameras list of objects to look at

	// Add GameObjects to the vector for render, updating etc.
	gameObjects.push_back(oilBarrel);

	// Enemy
	Enemy *enemyBarrel = new Enemy();
	enemyBarrel->setDistanceToEngage(100.0f);
	enemyBarrel->setGameObject(oilBarrel);
	enemyBarrel->setTarget(goMainCamera);

	enemies.push_back(enemyBarrel);

	//==============================================================================
	// ArmorDrecon Vehicle Model
	//==============================================================================

	modelPath = ASSET_PATH + MODEL_PATH + "armoredrecon.fbx";							// Set string for model address
	texturePath = ASSET_PATH + TEXTURE_PATH + "armoredrecon_diff.png";					// Set string for texture Map address
	heightMap = ASSET_PATH + TEXTURE_PATH + "armoredrecon_Height.png";					// Set string for height map address
	specularMap = ASSET_PATH + TEXTURE_PATH + "armoredrecon_spec.png";					// Set string for specular map address

	//Initialise GAMEOBJECT that will HOLD THE ARMORDRECON VEHICLE
	GameObject * armordrecon = loadFBXFromFile(modelPath, vec3(1.0f,1.0f,1.0f));		// Load model ARMORDRECON VEHICLE
	armordrecon->gameObject = armordrecon;												// link with self
	armordrecon->setName("armordrecon");												// Set name property in armordrecon's GameObject
	t = new Transform();																// Define new Transform for armordrecon's GameObject
	t->setPosition(0.0f,-1.0f, 0.0f);													// Set Transform position
	armordrecon->setTransform(t);														// Set Transform component onto the GameObject
	//goMainLight->setLight(light);														// Set GameObjects light component to the main light
	vsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureVS.glsl";				// Set string for Vertex Shader
	fsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureFS.glsl";				// Set string for Fragment Shader

	//For all child objects attach a new MATERIAL LOAD VERTEX and FRAGMENT SHADER
	for (int i = 0; i < armordrecon->getChildCount(); i++)
	{
		// Set MATERIAL Properties
		Material * material = new Material();											// Instantiate new material
		material->init();																// Initiate material
		material->loadDiffuseMap(texturePath);											// Load Diffuse Texture
		material->loadHeightMap(heightMap);												// Load Height Map
		material->loadSpecularMap(specularMap);											// Load Specular Map
		material->setSpecularPower(50.0f);												// Set Specular power for material
		material->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);							// Set Specular colour for material
		material->loadShader(vsPath, fsPath);											// Load Vertex and Fragment Shader onto the material

		armordrecon->getChild(i)->setMaterial(material);								// Set child material
	}

	// Initialise BOX COLLIDER for ArmorDrecon						
	boxCollider = new BoxCollider(5.3f, 5.2f, 2.5f);									// Instantiate Box Collider
	boxCollider->setParent(armordrecon);												// Set Box Collider parent as the ArmorDrecon's GameObject
	boxCollider->createColliderVertices();												// Initialise the box collider properties
	armordrecon->setCollider(static_cast<Collider*>(boxCollider));						// Set the Collider component on the armordrecons as a cast box collider
	basicCameraController->addGameObjectToTargets(armordrecon);							// Add the GameObjects the cameras list of objects to look at

	//Add GameObjects to the vector for render, updating etc.
	gameObjects.push_back(armordrecon);

	//==============================================================================
	// Cube Model
	//==============================================================================

	modelPath = ASSET_PATH + MODEL_PATH + "sun.fbx";				// Set string for model address
	texturePath = ASSET_PATH + TEXTURE_PATH + "sunTexture.png";			// Set string for texture Map address
	//heightMap = ASSET_PATH + TEXTURE_PATH + "";					// Set string for height map address
	//specularMap = ASSET_PATH + TEXTURE_PATH + "";					// Set string for specular map address

																						//Initialise GAMEOBJECT that will HOLD THE ARMORDRECON VEHICLE
	GameObject * goCube = loadFBXFromFile(modelPath, vec3(1.0f, 1.0f, 1.0f));				// Load model ARMORDRECON VEHICLE
	goCube->gameObject = goCube;															// Link with self
	goCube->setName("cubeFBX");															// Set name property in armordrecon's GameObject
	t = new Transform();															// Define new Transform for armordrecon's GameObject
	float fOrbitDistance = 300.0f;
	t->setRotation(90.0f, 0.0f, 0.0f);
	t->setPosition(-5.0f, 0.0f, fOrbitDistance);													// Set Transform position
	goCube->setTransform(t);																// Set Transform component onto the GameObject
	//goMainLight->setLight(light);														// Set GameObjects light component to the main light
	vsPath = ASSET_PATH + SHADER_PATH + "/textureVS.glsl";				// Set string for Vertex Shader
	fsPath = ASSET_PATH + SHADER_PATH + "/textureFS.glsl";				// Set string for Fragment Shader

	Planet *planetSun = new Planet(fOrbitDistance);
	planetSun->setGameObject(goCube);
	planets.push_back(planetSun);

	//For all child objects attach a new MATERIAL LOAD VERTEX and FRAGMENT SHADER
	for (int i = 0; i < goCube->getChildCount(); i++)
	{
		// Set MATERIAL Properties
		Material * material = new Material();											// Instantiate new material
		material->init();																// Initiate material
		material->loadDiffuseMap(texturePath);										// Load Diffuse Texture
		//material->loadHeightMap(heightMap);											// Load Height Map
		//material->loadSpecularMap(specularMap);										// Load Specular Map
		material->setSpecularPower(10.0f);											// Set Specular power for material
		material->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);							// Set Specular colour for material
		material->setDiffuseColour(1.0f,1.0f,1.0f,1.0f);
		material->loadShader(vsPath, fsPath);											// Load Vertex and Fragment Shader onto the material

		goCube->getChild(i)->setMaterial(material);										// Set child material
	}

	// Initialise BOX COLLIDER for ArmorDrecon						
	boxCollider = new BoxCollider(1.0f, 1.0f, 1.0f);									// Instantiate Box Collider
	boxCollider->setParent(goCube);														// Set Box Collider parent as the ArmorDrecon's GameObject
	boxCollider->createColliderVertices();												// Initialise the box collider properties
	goCube->setCollider(static_cast<Collider*>(boxCollider));						// Set the Collider component on the armordrecons as a cast box collider
	basicCameraController->addGameObjectToTargets(goCube);								// Add the GameObjects the cameras list of objects to look at

	//Add GameObjects to the vector for render, updating etc.
	gameObjects.push_back(goCube);

	//==============================================================================
	// Custom Shape - Box 1
	//==============================================================================

	// Instantiate GAMEOBJECT to hold CUSTOM SHAPE
	GameObject *goShape = new GameObject();												// Instantiate GameObject for Custom Shape
	goShape->gameObject = goShape;														// Link with self
	goShape->setName("Cube");															// Set name property in Custom Shape's GameObject
	//goShape->setLight(light);															// Set light component
	t = new Transform();																// Define new transform for Custom Shape
	t->setPosition(-5.0f, 1.0f, 8.0f);
	goShape->setTransform(t);															// Set Transform component in Custom Shape's GameObject

	// Instantiate CUSTOMSHAPE
	CustomShape customShape = CustomShape();
	customShape.init(goShape);

	Material *newMat = new Material();
	newMat->init();
	newMat->loadDiffuseMap("../assets/textures/grey.png");
	newMat->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	vsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureVS.glsl";
	fsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureFS.glsl";
	newMat->loadShader(vsPath, fsPath);
	goShape->setMaterial(newMat);

	// Instantiate BOX COLLIDER for Custom Shape - Box
	BoxCollider *newBoxCollider = new BoxCollider(2.0f,2.0f,2.0f);						// Instantiate SphereCollider with a radius
	//newBoxCollider->setOffSet(vec3(0.0f,-1.0f,0.0f));
	newBoxCollider->setParent(goShape);
	goShape->setCollider(static_cast<Collider*>(newBoxCollider));						// Set the Collider component on the Custom Shape : Box as a cast sphere collider
	basicCameraController->addGameObjectToTargets(goShape);								// Add the GameObjects the cameras list of objects to look at

	//Add GameObjects to the vector for render, updating etc.
	gameObjects.push_back(goShape);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	iLevelSize = 50;
	iBuildDirection = 0;
	v3BuildPosition = V3_START_POSITION - vec3(0.0f, V3_START_POSITION.y + 2.5f, 0.0f);

	int i = iLevelSize;				// Counter for placing path segments
	int iTurnCount = rand() % 10;	// Counter for changing the build direction

	//Generate path with a level size of segments
	while (i > 0) 
	{
		instantiateFloorSegment(v3BuildPosition);
		incrementBuildPosition();
		i--;
		iTurnCount--;
		if (iTurnCount == 0) 
		{
			rotateBuildDirection();
			iTurnCount = rand() % 10;
			std::cout << "Random Turn Count " << iTurnCount << std::endl;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Start Game Clock
	Timer::getTimer().start();
}

void SceneManager::createSkyBox()
{
	Vertex triangleData[] = {
		{ vec3(-10.0f, 10.0f, 10.0f) },// Top Left
		{ vec3(-10.0f, -10.0f, 10.0f) },// Bottom Left
		{ vec3(10.0f, -10.0f, 10.0f) }, //Bottom Right
		{ vec3(10.0f, 10.0f, 10.0f) },// Top Right


									  //back
		{ vec3(-10.0f, 10.0f, -10.0f) },// Top Left
		{ vec3(-10.0f, -10.0f, -10.0f) },// Bottom Left
		{ vec3(10.0, -10.0f, -10.0f) }, //Bottom Right
		{ vec3(10.0f, 10.0f, -10.0f) }// Top Right
	};


	GLuint indices[] = {
		//front
		0, 1, 2,
		0, 3, 2,

		//left
		4, 5, 1,
		4, 1, 0,

		//right
		3, 7, 2,
		7, 6, 2,

		//bottom
		1, 5, 2,
		6, 2, 1,

		//top
		5, 0, 7,
		5, 7, 3,

		//back
		4, 5, 6,
		4, 7, 6
	};

	//creat mesh and copy in

	Mesh * pMesh = new Mesh();
	pMesh->init();

	pMesh->copyVertexData(8, sizeof(Vertex), (void**)triangleData);
	pMesh->copyIndexData(36, sizeof(int), (void**)indices);

	Transform *t = new Transform();
	t->setPosition(0.0f, 0.0f, 0.0f);
	//load textures and skybox material + Shaders
	SkyBoxMaterial *material = new SkyBoxMaterial();
	material->init();

	std::string vsPath = ASSET_PATH + SHADER_PATH + "/skyVS.glsl";
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/skyFS.glsl";
	material->loadShader(vsPath, fsPath);

	std::string posZTexturename = ASSET_PATH + TEXTURE_PATH + "CloudyLightRaysFront2048.png";
	std::string negZTexturename = ASSET_PATH + TEXTURE_PATH + "CloudyLightRaysBack2048.png";
	std::string posXTexturename = ASSET_PATH + TEXTURE_PATH + "CloudyLightRaysLeft2048.png";
	std::string negXTexturename = ASSET_PATH + TEXTURE_PATH + "CloudyLightRaysRight2048.png";
	std::string posYTexturename = ASSET_PATH + TEXTURE_PATH + "CloudyLightRaysUp2048.png";
	std::string negYTexturename = ASSET_PATH + TEXTURE_PATH + "CloudyLightRaysDown2048.png";

	material->loadCubeTexture(posZTexturename, negZTexturename, posXTexturename, negXTexturename, posYTexturename, negYTexturename);
	//create gameobject but don't add to queue!
	skyBox = new GameObject();
	skyBox->setMaterial(material);
	skyBox->setTransform(t);
	skyBox->setMesh(pMesh);
}

void SceneManager::incrementBuildPosition()
{
	switch (iBuildDirection)
	{
		case 0:
			v3BuildPosition = v3BuildPosition + vec3(0.0f, 0.0f, -2.0f);
			break;
		case 1:
			v3BuildPosition = v3BuildPosition + vec3(-2.0f, 0.0f, 0.0f);
			break;
		case 2:
			v3BuildPosition = v3BuildPosition + vec3(0.0f, 0.0f, 2.0f);
			break;
		case 3:
			v3BuildPosition = v3BuildPosition + vec3(2.0f, 0.0f, 0.0f);
			break;
	}
}
void SceneManager::rotateBuildDirection()
{
	int iRand = rand() % 10; // get random number between 0-10
	if (iRand > 5) iBuildDirection++; // 50-50 chance
	else iBuildDirection--;	// set direction based on high low

	//Limit Direction to 0-3
	if (iBuildDirection > 3) iBuildDirection = 0;
	if (iBuildDirection < 0) iBuildDirection = 3;

	std::cout << "Build Direction now is " << iBuildDirection << std::endl;
}
void SceneManager::instantiateFloorSegment(vec3 v3InstantiatePosition)
{
	std::string vsPath;				// Holds string for Vertex Shader address
	std::string fsPath;				// Holds string for Fragment Shader address

									//==============================================================================
									// Cube Model
									//==============================================================================

	std::string modelPath = ASSET_PATH + MODEL_PATH + "cube.fbx";				// Set string for model address
	std::string texturePath = ASSET_PATH + TEXTURE_PATH + "grey.png";			// Set string for texture Map address
	//heightMap = ASSET_PATH + TEXTURE_PATH + "";					// Set string for height map address
	//specularMap = ASSET_PATH + TEXTURE_PATH + "";					// Set string for specular map address

																	//Initialise GAMEOBJECT that will HOLD THE ARMORDRECON VEHICLE
	GameObject * goCube = loadFBXFromFile(modelPath, vec3(25.0f, 25.0f, 25.0f));				// Load model ARMORDRECON VEHICLE
	goCube->gameObject = goCube;															// Link with self
	goCube->setName("cubeFBX");															// Set name property in armordrecon's GameObject
	Transform *t = new Transform();																// Define new Transform for armordrecon's GameObject
	t->setRotation(270.0f, 0.0f, 0.0f);
	t->setPosition(v3InstantiatePosition.x, v3InstantiatePosition.y, v3InstantiatePosition.z);													// Set Transform position
	goCube->setTransform(t);																// Set Transform component onto the GameObject
																							//goMainLight->setLight(light);														// Set GameObjects light component to the main light
	vsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureVS.glsl";				// Set string for Vertex Shader
	fsPath = ASSET_PATH + SHADER_PATH + "/DirectionalLightTextureFS.glsl";				// Set string for Fragment Shader

																						//For all child objects attach a new MATERIAL LOAD VERTEX and FRAGMENT SHADER
	for (int i = 0; i < goCube->getChildCount(); i++)
	{
		// Set MATERIAL Properties
		Material * material = new Material();											// Instantiate new material
		material->init();																// Initiate material
		material->loadDiffuseMap("../assets/textures/grey.png");										// Load Diffuse Texture
																										//material->loadHeightMap(heightMap);											// Load Height Map
																										//material->loadSpecularMap(specularMap);										// Load Specular Map
		material->setSpecularPower(10.0f);											// Set Specular power for material
		material->setSpecularColour(1.0f, 1.0f, 0.0f, 1.0f);							// Set Specular colour for material
		material->setDiffuseColour(0.3f, 0.3f, 0.3f, 1.0f);
		material->loadShader(vsPath, fsPath);											// Load Vertex and Fragment Shader onto the material

		goCube->getChild(i)->setMaterial(material);										// Set child material
	}

	// Initialise BOX COLLIDER for ArmorDrecon						
	BoxCollider *boxCollider = new BoxCollider(2.0f, 2.0f, 2.0f);									// Instantiate Box Collider
	boxCollider->setOffSet(vec3(0.0f,0.5f,0.0f));
	boxCollider->setParent(goCube);														// Set Box Collider parent as the ArmorDrecon's GameObject
	boxCollider->createColliderVertices();												// Initialise the box collider properties
	goCube->setCollider(static_cast<Collider*>(boxCollider));						// Set the Collider component on the armordrecons as a cast box collider
	basicCameraController->addGameObjectToTargets(goCube);								// Add the GameObjects the cameras list of objects to look at

																						//Add GameObjects to the vector for render, updating etc.
	gameObjects.push_back(goCube);

	//Add GameObjects to the vector for render, updating etc.
	gameObjects.push_back(goCube);
	floorTiles.push_back(goCube);

	std::cout << "Placed Floor Tile (" << v3BuildPosition.x << "," << v3BuildPosition.y << "," << v3BuildPosition.z << ")" << std::endl;
}