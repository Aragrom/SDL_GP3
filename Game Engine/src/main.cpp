#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY

#include "GameConstants.h"
#include "FontManager.h"
#include "SoundManager.h"

#include "windowOGL.h"
#include "Colours.h"
#include "Shapes.h"
#include "Pyramid.h"
#include "Cube.h"
#include "Sphere.h"

#include "cMaterial.h"

OpenGLLoader *openGLLoader;
PostProcessing postProcessor;
SceneManager *sceneManager;

int iGraphicSystem = OpenGL;
bool bRunning = true;
bool bDebug = false;

windowOGL windOGL;

// Sphere
Sphere thePlanet;
Texture planetTexture;
cMaterial planetMaterial;
float fRotSpeed;

//==============================================================================
// Handles the releasing of memory for required resources
//==============================================================================
void cleanUp()
{
	sceneManager->cleanUp();
	postProcessor.destroy();
	// clean up, reverse order!!!
	Input::getInput().destroy();

	windOGL.shutdown(); //Free any resources
	openGLLoader->cleanUp();
}

//==============================================================================
// Initialises the input using an address to a text file representing a Database
//==============================================================================
void initInput()
{
    const std::string inputDBFilename=ASSET_PATH+"gamecontrollerdb.txt";
    Input::getInput().init(inputDBFilename);
}

//==============================================================================
// Main Update Function - Updates game states
//==============================================================================
void update()
{
    Timer::getTimer().update();

	sceneManager->updateGameObjects();
	sceneManager->updatePlanets();
	sceneManager->updatePlayer();

	Input::getInput().update();
}

//==============================================================================
// Render the GameObject parameter and its associated children
//==============================================================================
void renderGameObject(GameObject * pObject)
{
	if (!pObject)
		return;

	pObject->render();	// Call GameObject render()

	Mesh * currentMesh = pObject->getMesh();						// Get GameObject Mesh
	Transform * currentTransform = pObject->getTransform();			// Get GameObject Transform
	Material * currentMaterial = (Material*)pObject->getMaterial();	// Get GameObject Material

	if (currentMesh && currentMaterial && currentTransform)
	{
		currentMaterial->bind();
		currentMesh->bind();

		// Get uniforms values from bound material
		GLint MVPLocation = currentMaterial->getUniformLocation("MVP");
		GLint ModelLocation = currentMaterial->getUniformLocation("Model");
		GLint ambientMatLocation = currentMaterial->getUniformLocation("ambientMaterialColour");
		GLint ambientLightLocation = currentMaterial->getUniformLocation("ambientLightColour");
		GLint diffuseMatLocation = currentMaterial->getUniformLocation("diffuseMaterialColour");
		GLint diffuseLightLocation = currentMaterial->getUniformLocation("diffuseLightColour");
		GLint lightDirectionLocation = currentMaterial->getUniformLocation("lightDirection");
		GLint specularMatLocation = currentMaterial->getUniformLocation("specularMaterialColour");
		GLint specularLightLocation = currentMaterial->getUniformLocation("specularLightColour");
		GLint specularpowerLocation = currentMaterial->getUniformLocation("specularPower");
		GLint cameraPositionLocation = currentMaterial->getUniformLocation("cameraPosition");
		GLint diffuseTextureLocation = currentMaterial->getUniformLocation("diffuseMap");
		GLint specTextureLocation = currentMaterial->getUniformLocation("specMap");
		GLint bumpTextureLocation = currentMaterial->getUniformLocation("bumpMap");
		GLint heightTextureLocation = currentMaterial->getUniformLocation("heightMap");

		// From SceneManager Object
		Camera * cam = sceneManager->getMainCamera()->getCamera();	// Get Main Camera
		Light* light = sceneManager->getMainLight()->getLight();	// Get Main Light

		// Get the MODEL VIEW PROJECTION
		mat4 MVP = cam->getProjection()*cam->getView()*currentTransform->getModel();
		mat4 Model = currentTransform->getModel();

		// Get Material Properties for effecting fragment colour
		vec4 ambientMaterialColour = currentMaterial->getAmbientColour();
		vec4 diffuseMaterialColour = currentMaterial->getDiffuseColour();
		vec4 specularMaterialColour = currentMaterial->getSpecularColour();
		float specularPower = currentMaterial->getSpecularPower();

		// Get Light Properties for effecting fragment colour
		vec4 diffuseLightColour = light->getDiffuseColour();
		vec4 specularLightColour = light->getSpecularColour();

		vec3 lightDirection = light->getDirection();											// Get Main Light Direction
		vec3 cameraPosition = sceneManager->getMainCamera()->getTransform()->getPosition();		// Get Main Camera Position

		// Define Uniforms for Drawing Elements
		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform4fv(ambientMatLocation, 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(ambientLightLocation, 1, glm::value_ptr(vec4(1.0f, 1.0f, 1.0f, 1.0f)));

		glUniform4fv(diffuseMatLocation, 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(diffuseLightLocation, 1, glm::value_ptr(diffuseLightColour));
		glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));

		glUniform4fv(specularMatLocation, 1, glm::value_ptr(specularMaterialColour));
		glUniform4fv(specularLightLocation, 1, glm::value_ptr(specularLightColour));

		glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));
		glUniform1f(specularpowerLocation, specularPower);

		glUniform1i(diffuseTextureLocation, 0);
		glUniform1i(specTextureLocation, 1);
		glUniform1i(bumpTextureLocation, 2);
		glUniform1i(heightTextureLocation, 3);

		// !!!- DRAW - !!!
		glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);

		// Unbind material
		currentMaterial->unbind();
	}

	// For each child object render
	for (int i = 0; i < pObject->getChildCount(); i++)
	{
		renderGameObject(pObject->getChild(i));
	}
}

//Function to render(aka draw)
void render(FontManager* fontManager)
{
	//Bind Framebuffer
	//postProcessor.bind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<GameObject*> gameObjects = sceneManager->getGameObjects();

	//openGLLoader->setProjection3D(iWINDOW_WIDTH, iWINDOW_HEIGHT);

	//alternative sytanx
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		renderGameObject((*iter));
	}

	//windOGL.initOGL(iWINDOW_WIDTH,iWINDOW_HEIGHT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//thePlanet.setRotAngle(thePlanet.getRotAngle() + 0.0f);
	//thePlanet.prepare(thePlanet.getRotAngle());					//Do any pre-rendering logic
	//planetMaterial.useMaterial();									// Set the material for use
	//thePlanet.render(thePlanet.getRotAngle());

	//glPushMatrix();
	//openGLLoader->setOrtho2D(iWINDOW_WIDTH, iWINDOW_HEIGHT);
	//fontManager->getFont("DrWho")->printText("Tardis Wars", FTPoint(0.0f, 0.0f, 0.0f), Colour3f(0.0f, 255.0f, 0.0f));
	//theFontMgr->getFont("DrWho")->printText(outputMsg.c_str(), FTPoint(850, 35, 0.0f), Colour3f(255.0f, 255.0f, 0.0f)); // uses c_str to convert string to LPCSTR
	//glPopMatrix();

	//now switch to normal framebuffer
	//postProcessor.preDraw();
	//Grab stuff from shader
	GLint colourFilterLocation = postProcessor.getUniformVariableLocation("colourFilter");
	glUniformMatrix3fv(colourFilterLocation, 1, GL_FALSE, glm::value_ptr(SEPIA_FILTER));

	//draw
	//postProcessor.draw();

	//post draw
	//postProcessor.postDraw();

	SDL_GL_SwapWindow(openGLLoader->getWindow());
}

//Main Method
int main(int argc, char * arg[])
{
	if (iGraphicSystem == OpenGL)
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
		if (((returnInitFlags)& (imageInitFlags)) != imageInitFlags) {
			std::cout << "ERROR SDL_Image Init " << IMG_GetError() << std::endl;
			// handle error
		}

		if (TTF_Init() == -1) {
			std::cout << "TTF_Init: " << TTF_GetError();
		}

		if (!windOGL.initOGL(iWINDOW_WIDTH, iWINDOW_HEIGHT)) //Initialize our example
		{
			std::cout << "Could not initialize the application" << std::endl;
			return 1;
		}

		// Font manager
		FontManager *fontManager = FontManager::getInstance();
		// Load Fonts
		LPCSTR gameFonts[3] = { "../assets/fonts/digital-7.ttf", "../assets/fonts/space age.ttf", "../assets/fonts/doctor_who.ttf" };
		fontManager->addFont("SevenSeg", gameFonts[0], 24);
		fontManager->addFont("Space", gameFonts[1], 24);
		fontManager->addFont("DrWho", gameFonts[2], 48);

		// This is the sound manager
		static SoundManager* soundManager = SoundManager::getInstance();
		// Load Sound
		LPCSTR gameSounds[3] = { "../assets/Audio/who10Edit.wav", "../assets/Audio/shot007.wav", "../assets/Audio/explosion2.wav" };

		soundManager->add("Theme", gameSounds[0]);
		soundManager->add("Shot", gameSounds[1]);
		soundManager->add("Explosion", gameSounds[2]);

		soundManager->getSnd("Theme")->playAudio(AL_LOOPING); // Start background music

		openGLLoader = new OpenGLLoader();
		openGLLoader->initWindow(iWINDOW_WIDTH, iWINDOW_HEIGHT, false);
		//Call our InitOpenGL Function
		openGLLoader->initOpenGL();
		openGLLoader->checkForErrors();
		//Set our viewport
		openGLLoader->setViewport(iWINDOW_WIDTH, iWINDOW_HEIGHT);
		initInput();

		// Sphere
		thePlanet = Sphere(3, 30, 30);
		planetTexture.createTexture("../assets/Images/Earth.png");
		fRotSpeed = 3.0f;
		thePlanet.initialise(planetTexture.getTexture(), glm::vec3(0, 0, 20), glm::vec3(0, 0, 0));
		planetMaterial= cMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
		
		//Initialise();
		sceneManager = new SceneManager();
		sceneManager->initScene();

		//Game Loop
		while (openGLLoader->isActive())
		{
			openGLLoader->handleEvents();
			//Grab input
			if (Input::getInput().getKeyboard()->isKeyDown(SDLK_f)) // Sound Test ####################################################
			{
				std::cout << "Shot Fired" << std::endl;
				soundManager->getSnd("Shot")->playAudio(AL_TRUE);
			}
			std::thread updateThread(update);				//Thread game logic and collision
			//updateThread.detach();
			updateThread.join();
			
			render(fontManager);
		}
		cleanUp();
	}

	return 0;
}