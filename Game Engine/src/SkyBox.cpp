#include "GameConstants.h"

void SkyBox::createSkyBox(OpenGLLoader *openGLLoader, GameObject *goSkyBox)
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
	goSkyBox = new GameObject();
	goSkyBox->setMaterial(material);
	goSkyBox->setTransform(t);
	goSkyBox->setMesh(pMesh);

	openGLLoader->checkForErrors();
}