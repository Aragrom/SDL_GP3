#include <iostream>
#include <GL/glew.h>
//maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/glu.h>
#include <CoreFoundation/CoreFoundation.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#elif WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <gl/GLU.h>
#endif

#include <vector>

#ifdef _DEBUG && WIN32
const std::string ASSET_PATH = "../assets/";
const std::string SHADER_PATH = "shaders/";
const std::string TEXTURE_PATH = "textures/";
const std::string FONT_PATH = "fonts/";
const std::string MODEL_PATH = "models/";
#elif __APPLE__
const std::string ASSET_PATH;
const std::string SHADER_PATH;
const std::string TEXTURE_PATH;
const std::string FONT_PATH;
#else
const std::string ASSET_PATH = "/assets/";
const std::string SHADER_PATH = "shaders/";
#endif

//headers
#include "FbxLoader.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"
#include "CXBOXController.h"
#include <btBulletDynamicsCommon.h> //for physics

//xbox controller
CXBOXController* cxboxController;

//bool for displaying instructions
bool bControllerIsConnected = false;
bool bDisplayedInstructions = false;

GLUquadricObj* GLU_quadricObj;
btDynamicsWorld* bt_DynamicsWorld; //every physical object go to the world
btDispatcher* bt_Dispatcher; // Used in collision algorithm
btCollisionConfiguration* bt_CollisionConfiguration;	//Used in collision algorithm
btBroadphaseInterface* bt_BroadphaseInterface;	//should Bullet examine every object, or just what close to each other
btConstraintSolver* bt_ConstraintSolver; //solve collisions, apply forces, impulses

std::vector<btRigidBody*> vbt_RigidBody;

std::vector<GameObject*> vGameObjects;

//SDL Window
SDL_Window * sdl_window = NULL;
//SDL GL Context
SDL_GLContext sdl_glcontext = NULL;

//Window Width
const int iWINDOW_WIDTH = 640;
//Window Height
const int iWINDOW_HEIGHT = 480;

bool bRunning = true;

vec4 v4AmbientLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

// Main Camera
GameObject * goMainCamera;

GameObject * goMainLight;

Vertex triangleData[] = {
	{ vec3(-0.5f, 0.5f, 0.5f), vec3(0.25f, 0.25f, 0.5f), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Top Left
	{ vec3(-0.5f, -0.5f, 0.5f), vec3(0.25f, 0.25f, 0.5f), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },// Bottom Left
	{ vec3(0.5f, -0.5f, 0.5f), vec3(0.25f, -0.25f, 0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, //Bottom Right
	{ vec3(0.5f, 0.5f, 0.5f), vec3(0.25f, -0.25f, 0.5f), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Top Right


	//back
	{ vec3(-0.5f, 0.5f, -0.5f), vec3(0.25f, 0.25f, -0.5f), vec2(0.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },// Top Left
	{ vec3(-0.5f, -0.5f, -0.5f), vec3(0.25f, 0.25f, -0.5f), vec2(0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Bottom Left
	{ vec3(0.5f, -0.5f, -0.5f), vec3(0.25f, -0.25f, -0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f) }, //Bottom Right
	{ vec3(0.5f, 0.5f, -0.5f), vec3(0.25f, -0.25f, -0.5f), vec2(1.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) }// Top Right
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

void CheckForErrors()
{
	GLenum error;
	do{
		error = glGetError();
	} while (error != GL_NO_ERROR);
}

btRigidBody* AddSphere(float rad, float x, float y, float z, float mass)
{
	btTransform t;	//position and rotation
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z)); //put it to x,y,z coordinates
	btSphereShape* bt_SphereShape = new btSphereShape(rad);	//it's a sphere, so use sphereshape
	btVector3 inertia(0, 0, 0);	//inertia is 0,0,0 for static object, else
	if (mass != 0.0)
		bt_SphereShape->calculateLocalInertia(mass, inertia);	//it can be determined by this function (for all kind of shapes)

	btMotionState* bt_MotionState = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo bt_RigidBodyConstructionInfo(0.0, bt_MotionState, bt_SphereShape);
	btRigidBody* bt_rigidbody = new btRigidBody(bt_RigidBodyConstructionInfo);
	bt_DynamicsWorld->addRigidBody(bt_rigidbody);
	vbt_RigidBody.push_back(bt_rigidbody);
	return bt_rigidbody;
}

//Global functions
void InitWindow(int width, int height, bool fullscreen)
{
	//Create a window
	sdl_window = SDL_CreateWindow(
		"OpenGL C++ Game Engine",   // window title
		SDL_WINDOWPOS_CENTERED,     // x position, centered
		SDL_WINDOWPOS_CENTERED,     // y position, centered
		width,                      // width, in pixels
		height,                     // height, in pixels
		SDL_WINDOW_OPENGL           // flags
		);
}

void CleanUp()
{
	auto iter = vGameObjects.begin();
	while (iter != vGameObjects.end())
	{
		(*iter)->Destroy();
		if ((*iter))
		{
			delete (*iter);
			(*iter) = NULL;
			iter = vGameObjects.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	vGameObjects.clear();

	//x-box controller memory release
	delete(cxboxController);

	//Bullet physics memory release
	for (int i = 0; i<vbt_RigidBody.size(); i++)
	{
		bt_DynamicsWorld->removeCollisionObject(vbt_RigidBody[i]);
		btMotionState* bt_motionState = vbt_RigidBody[i]->getMotionState();
		btCollisionShape* bt_collisionShape = vbt_RigidBody[i]->getCollisionShape();
		delete vbt_RigidBody[i];
		delete bt_collisionShape;
		delete bt_motionState;
	}
	delete bt_Dispatcher;
	delete bt_CollisionConfiguration;
	delete bt_ConstraintSolver;
	delete bt_BroadphaseInterface;
	delete bt_DynamicsWorld;
	gluDeleteQuadric(GLU_quadricObj);

	// clean up, reverse order!!!
	//Open GL memory release
	SDL_GL_DeleteContext(sdl_glcontext);
	SDL_DestroyWindow(sdl_window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

//Function to initialise OpenGL
void InitOpenGL()
{
	//Ask for version 3.2 of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create OpenGL Context
	sdl_glcontext = SDL_GL_CreateContext(sdl_window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	//Smooth shading
	glShadeModel(GL_SMOOTH);

	//clear the background to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Clear the depth buffer
	glClearDepth(1.0f);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//The depth test to go
	glDepthFunc(GL_LEQUAL);

	//Turn on best perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

//Function to set/reset viewport
void SetViewport(int width, int height)
{

	//make sure height is always above 1
	if (height == 0) {
		height = 1;
	}


	//Setup viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void Initialise()
{	
	//Initialise a "GameObject" to hold Main Camera
	goMainCamera = new GameObject();

	goMainCamera->SetName("MainCamera"); //Name camera

	//Initialise a "Transform component"
	Transform *t = new Transform();

	//Set x,y,z of "Transform component"
	t->SetPosition(0.0f, 0.0f, 0.0f);

	//Assign "Transform component" to "GameObject"
	goMainCamera->SetTransform(t);

	Camera * c = new Camera(); //Initialise a "Camera component"

	//Set Camera component properties
	c->SetAspectRatio((float)(iWINDOW_WIDTH / iWINDOW_HEIGHT));
	c->SetFOV(45.0f);
	c->SetNearClip(0.1f);
	c->SetFarClip(1000.0f);

	//Assign created "Camera component" to "Main Camera GameObject" Camera variable
	goMainCamera->SetCamera(c);

	//Add camera to Vector that holds GameObjects
	vGameObjects.push_back(goMainCamera);
	
	goMainLight = new GameObject();
	goMainLight->SetName("MainLight");

	t = new Transform();
	t->SetPosition(0.0f, 0.0f, 0.0f);
	goMainLight->SetTransform(t);

	Light * light = new Light();
	goMainLight->SetLight(light);
	vGameObjects.push_back(goMainLight);


	GameObject * goCube = new GameObject();
	goCube->SetName("Cube");
	Transform *transform = new Transform();
	transform->SetPosition(0.0f, 0.0f, 0.0f);
	goCube->SetTransform(transform);

	Material * material = new Material();
	std::string strVSPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
	std::string strFSPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
	material->LoadShader(strVSPath, strFSPath);
	goCube->SetMaterial(material);

	Mesh * mesh = new Mesh();
	goCube->SetMesh(mesh);
	vGameObjects.push_back(goCube);


	//alternative sytanx
	for (auto iter = vGameObjects.begin();
		iter != vGameObjects.end();
		iter++)
	{
		(*iter)->Init();
	}

	mesh->CopyVertexData(8, sizeof(Vertex), (void**)triangleData);
	mesh->CopyIndexData(36, sizeof(int), (void**)indices);


	std::string modelPath = ASSET_PATH + MODEL_PATH + "armoredrecon.fbx";
	GameObject * go = loadFBXFromFile(modelPath);
	for (int i = 0; i < go->GetChildCount(); i++)
	{
		Material * material = new Material();
		material->Init();
		std::string vsPath = ASSET_PATH + SHADER_PATH + "/ambientVS.glsl";
		std::string fsPath = ASSET_PATH + SHADER_PATH + "/ambientFS.glsl";
		material->LoadShader(vsPath, fsPath);

		go->GetChild(i)->SetMaterial(material);
	}
	vGameObjects.push_back(go);
}

void InitialiseBulletPhysics()
{
	bt_CollisionConfiguration = new btDefaultCollisionConfiguration();
	bt_Dispatcher = new btCollisionDispatcher(bt_CollisionConfiguration);
	bt_BroadphaseInterface = new btDbvtBroadphase();
	bt_ConstraintSolver = new btSequentialImpulseConstraintSolver();
	bt_DynamicsWorld = new btDiscreteDynamicsWorld(bt_Dispatcher, bt_BroadphaseInterface, bt_ConstraintSolver, bt_CollisionConfiguration);
	bt_DynamicsWorld->setGravity(btVector3(0, -10, 0));	//gravity on Earth

	//Intialise Plane
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* bt_StaticPlaneShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState* bt_MotionState = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo bt_RigidBodyConstructionInfo(0.0, bt_MotionState, bt_StaticPlaneShape);
	btRigidBody* bt_rigidbody = new btRigidBody(bt_RigidBodyConstructionInfo);
	bt_DynamicsWorld->addRigidBody(bt_rigidbody);
	vbt_RigidBody.push_back(bt_rigidbody);

	AddSphere(1.0, 0, 20, 0, 1.0);

	GLU_quadricObj = gluNewQuadric();
}

void RenderPlane(btRigidBody* bt_RigidBody)
{
	if (bt_RigidBody->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
		return;
	glColor3f(0.8, 0.8, 0.8);
	btTransform t;
	bt_RigidBody->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
	glMultMatrixf(mat);	//translation,rotation
	glBegin(GL_QUADS);
	glVertex3f(-1000, 0, 1000);
	glVertex3f(-1000, 0, -1000);
	glVertex3f(1000, 0, -1000);
	glVertex3f(1000, 0, 1000);
	glEnd();
	glPopMatrix();
}

void RenderSphere(btRigidBody* bt_RigidBody)
{
	if (bt_RigidBody->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)	//only render, if it's a sphere
		return;
	glColor3f(1, 0, 0);
	float r = ((btSphereShape*)bt_RigidBody->getCollisionShape())->getRadius();
	btTransform t;
	bt_RigidBody->getMotionState()->getWorldTransform(t);	//get the transform
	float mat[16];
	t.getOpenGLMatrix(mat);	//OpenGL matrix stores the rotation and orientation
	glPushMatrix();
	glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
	gluSphere(GLU_quadricObj, r, 20, 20);
	glPopMatrix();
}

void RenderGameObject(GameObject *pObject)
{
	if (!pObject)
		return;

	pObject->Render();

	Mesh * currentMesh = pObject->GetMesh();
	Transform * currentTransform = pObject->GetTransform();
	Material * currentMaterial = pObject->GetMaterial();

	if (currentMesh && currentMaterial && currentTransform)
	{
		currentMaterial->Bind();
		currentMesh->Bind();

		GLint MVPLocation = currentMaterial->GetUniformLocation("MVP");
		GLint ModelLocation = currentMaterial->GetUniformLocation("Model");
		GLint ambientMatLocation = currentMaterial->GetUniformLocation("ambientMaterialColour");
		GLint ambientLightLocation = currentMaterial->GetUniformLocation("ambientLightColour");
		GLint diffuseMatLocation = currentMaterial->GetUniformLocation("diffuseMaterialColour");
		GLint diffuseLightLocation = currentMaterial->GetUniformLocation("diffuseLightColour");
		GLint lightDirectionLocation = currentMaterial->GetUniformLocation("lightDirection");
		GLint specularMatLocation = currentMaterial->GetUniformLocation("specularMaterialColour");
		GLint specularLightLocation = currentMaterial->GetUniformLocation("specularLightColour");
		GLint specularpowerLocation = currentMaterial->GetUniformLocation("specularPower");
		GLint cameraPositionLocation = currentMaterial->GetUniformLocation("cameraPosition");

		Camera * cam = goMainCamera->GetCamera();
		Light* light = goMainLight->GetLight();


		mat4 MVP = cam->GetProjection()*cam->GetView()*currentTransform->GetModel();
		mat4 Model = currentTransform->GetModel();

		vec4 ambientMaterialColour = currentMaterial->GetAmbientColour();
		vec4 diffuseMaterialColour = currentMaterial->GetDiffuseColour();
		vec4 specularMaterialColour = currentMaterial->GetSpecularColour();
		float specularPower = currentMaterial->GetSpecularPower();

		vec4 diffuseLightColour = light->GetDiffuseColour();
		vec4 specularLightColour = light->GetSpecularColour();
		vec3 lightDirection = light->GetDirection();

		vec3 cameraPosition = goMainCamera->GetTransform()->GetPosition();

		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform4fv(ambientMatLocation, 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(ambientLightLocation, 1, glm::value_ptr(v4AmbientLightColour));

		glUniform4fv(diffuseMatLocation, 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(diffuseLightLocation, 1, glm::value_ptr(diffuseLightColour));
		glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));

		glUniform4fv(specularMatLocation, 1, glm::value_ptr(specularMaterialColour));
		glUniform4fv(specularLightLocation, 1, glm::value_ptr(specularLightColour));

		glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));
		glUniform1f(specularpowerLocation, specularPower);

		glDrawElements(GL_TRIANGLES, currentMesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < pObject->GetChildCount(); i++)
	{
		RenderGameObject(pObject->GetChild(i));
	}
}


/*
Function to render (draw)
*/
void Render()
{
	//old imediate mode!
	//Set the clear colour(background)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto iter = vGameObjects.begin();
		iter != vGameObjects.end();
		iter++)
	{
		RenderGameObject((*iter));
	}

	for (int i = 0; i<vbt_RigidBody.size(); i++)
	{
		if (vbt_RigidBody[i]->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
			RenderPlane(vbt_RigidBody[i]);
		else if (vbt_RigidBody[i]->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
			RenderSphere(vbt_RigidBody[i]);
	}

	SDL_GL_SwapWindow(sdl_window);
}

//Function to update the game state
void Update()
{
	//alternative sytanx
	for (auto iter = vGameObjects.begin(); iter != vGameObjects.end(); iter++)
	{
		(*iter)->Update();
	}
}

/*
Trying to build a deadzone check for left analog stick only
*/
void ControllerDeadZone()
{
	XINPUT_STATE state = cxboxController->GetState();

	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY*LY);

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}
}

//Main Method
int main(int argc, char * arg[])
{
	// Setup asset path, no real work required for Windows. Mac needs to load assets from a bundle
	// ----------------------------------------------------------------------------
	// http://stackoverflow.com/questions/516200/relative-paths-not-working-in-xcode-c
	// This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		// error!
	}
	CFRelease(resourcesURL);

	chdir(path);
	std::cout << "Current Path: " << path << std::endl;
#endif

	// init everyting - SDL, if it is nonzero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "ERROR SDL_Init " << SDL_GetError() << std::endl;

		return -1;
	}

	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if (((returnInitFlags)& (imageInitFlags)) != imageInitFlags)
	{
		std::cout << "ERROR SDL_Image Init " << IMG_GetError() << std::endl;
		// handle error
	}

	if (TTF_Init() == -1)
	{
		std::cout << "TTF_Init: " << TTF_GetError();
	}

	InitWindow(iWINDOW_WIDTH, iWINDOW_HEIGHT, false);

	//Call our InitOpenGL Function
	InitOpenGL();

	CheckForErrors();
	//Set our viewport
	SetViewport(iWINDOW_WIDTH, iWINDOW_HEIGHT);

	//Generate Game Data
	Initialise();

	//"Try" Initialise controller class as player "1"
	cxboxController = new CXBOXController(1);

	//Value to hold the event generated by SDL
	SDL_Event sdl_event;

	//Game Loop	
	#pragma region Game Loop
	while (bRunning)
	{		
		bt_DynamicsWorld->stepSimulation(1 / 60.0); //update simulation (FPS)
		
		if (cxboxController->IsConnected())
		{
			if (!bControllerIsConnected)
			{
				bControllerIsConnected = true;
				std::cout << "\n XBOX 360 Controller Has Been Found!\n";
				std::cout << "\nCamera Navigation:" << std::endl;
				std::cout << "[LEFT_ANALOG_UP] Move camera forward" << std::endl;
				std::cout << "[LEFT_ANALOG_DOWN] Move camera backward" << std::endl;
				std::cout << "[LEFT_TRIGGER] Rotate camera left" << std::endl;
				std::cout << "[RIGHT_TRIGGER] Rotate camera rightn" << std::endl;
				std::cout << "[RIGHT_ANALOG_UP] Move camera up" << std::endl;
				std::cout << "[RIGHT_ANALOG_DOWN] Move camera down \n" << std::endl;
			}

			ControllerDeadZone();

			if (cxboxController->GetState().Gamepad.wButtons
				& XINPUT_GAMEPAD_A)
			{

			}
			if (cxboxController->GetState().Gamepad.wButtons
				& XINPUT_GAMEPAD_B)
			{

			}
			if (cxboxController->GetState().Gamepad.wButtons
				& XINPUT_GAMEPAD_X)
			{

			}
			if (cxboxController->GetState().Gamepad.wButtons
				& XINPUT_GAMEPAD_Y)
			{

			}

			if (cxboxController->GetState().Gamepad.sThumbLY >
				7849)
			{
				goMainCamera->GetCamera()->MoveCamera("forward");
			}

			if (cxboxController->GetState().Gamepad.sThumbLY <
				-7849)
			{
				goMainCamera->GetCamera()->MoveCamera("backward");
			}

			if (cxboxController->GetState().Gamepad.sThumbRY >
				8689)
			{
				goMainCamera->GetCamera()->MoveCamera("up");
			}

			if (cxboxController->GetState().Gamepad.sThumbRY <
				-8689)
			{
				goMainCamera->GetCamera()->MoveCamera("down");
			}

			if (cxboxController->GetState().Gamepad.bLeftTrigger >
				30)
			{
				goMainCamera->GetCamera()->RotateCamera(-1);
			}

			if (cxboxController->GetState().Gamepad.bRightTrigger >
				30)
			{
				goMainCamera->GetCamera()->RotateCamera(1);
			}

			if (cxboxController->GetState().Gamepad.wButtons
				& XINPUT_GAMEPAD_BACK)
			{
				bRunning = false;
			}
		}
		else
		{
			if (!bDisplayedInstructions)
			{
				bDisplayedInstructions = true;
				std::cout << "\n!!! - An Xbox 360 Controller was not detected - !!! \n" << std::endl;
				std::cout << "\nCamera Navigation:" << std::endl;
				std::cout << "[W] Move camera forward" << std::endl;
				std::cout << "[S] Move camera backward" << std::endl;
				std::cout << "[A] Rotate camera left" << std::endl;
				std::cout << "[D] Rotate camera rightn" << std::endl;
				std::cout << "[SPACE] Move camera up" << std::endl;
				std::cout << "[LEFT_SHIFT] Move camera down \n" << std::endl;
			}
		}

		#pragma region Event Queue
		/* Peek and Poll: Peek is used to check the event but will not
		   remove it; Poll will check the event but will also remove it
		   from the queue
		   
		   If there are events in the queue then the function will return
		   a positive value. The function will also fill out the SDL_Event
		   variable with the details of the event. */
		while (SDL_PollEvent(&sdl_event))
		{
			#pragma region Event Procedures
			if (sdl_event.type == SDL_QUIT || sdl_event.type == SDL_WINDOWEVENT_CLOSE) {
				//set our boolean which controls the game loop to false
				bRunning = false;
			}

			switch (sdl_event.type)
			{
				case SDL_KEYDOWN:

					//http://wiki.libsdl.org/SDL_Keycode
					switch (sdl_event.key.keysym.scancode)
					{
						#pragma region Number Keys 0 - 4
						case SDL_SCANCODE_0:
							break;
						case SDL_SCANCODE_1:
							break;
						case SDL_SCANCODE_2:
							break;
						case SDL_SCANCODE_3:
							break;
						case SDL_SCANCODE_4:
							break;
						#pragma endregion

						#pragma region Number Keys W,A,S,D & SPACE
						case SDL_SCANCODE_A:
							goMainCamera->GetCamera()->RotateCamera(-1);
							break;
						
						case SDL_SCANCODE_D:
							goMainCamera->GetCamera()->RotateCamera(1);
							break;

						case SDL_SCANCODE_W:
							goMainCamera->GetCamera()->MoveCamera("forward");
							break;

						case SDL_SCANCODE_S:
							goMainCamera->GetCamera()->MoveCamera("backward");
							break;

						case SDL_SCANCODE_SPACE:
							goMainCamera->GetCamera()->MoveCamera("up");
							break;

						case SDL_SCANCODE_LSHIFT:
							goMainCamera->GetCamera()->MoveCamera("down");
							break;
						#pragma endregion

						default:
							break;
					}

				/*case SDL_KEYUP:

					switch (sdl_event.key.keysym.scancode)
					{
						#pragma region Number Keys 0 - 4
						case SDL_SCANCODE_0:
							std::cout << "0 Key Released" << std::endl;
							break;
					}*/

				default:
					break;
			}
			#pragma endregion

		}
		#pragma endregion

		Update();
		Render();
	}
	#pragma endregion


	//Release memory allocated
	CleanUp();

	return 0;
}

///////////////////////////////////////////////////////////
#pragma endregion


