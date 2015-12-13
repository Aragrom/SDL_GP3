#include "GameConstants.h"

OpenGLLoader *openGLLoader;
Renderer *renderer;
PostProcessing postProcessor;
SceneManager *sceneManager;
int iGraphicSystem = OpenGL;
bool bRunning = true;
bool bDebug = false;

void cleanUp()
{    
	sceneManager->cleanUp();
	postProcessor.destroy();
	// clean up, reverse order!!!
	Input::getInput().destroy();

	openGLLoader->cleanUp();
}

void initInput()
{
    const std::string inputDBFilename=ASSET_PATH+"gamecontrollerdb.txt";
    Input::getInput().init(inputDBFilename);
}

//Function to update the game state
void update()
{
    Timer::getTimer().update();

	sceneManager->updateGameObjects();

	Input::getInput().update();
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

		openGLLoader = new OpenGLLoader();
		openGLLoader->initWindow(iWINDOW_WIDTH, iWINDOW_HEIGHT, false);
		//Call our InitOpenGL Function
		openGLLoader->initOpenGL();
		openGLLoader->checkForErrors();
		//Set our viewport
		openGLLoader->setViewport(iWINDOW_WIDTH, iWINDOW_HEIGHT);
		renderer = new Renderer();
		initInput();
		//Initialise();
		sceneManager = new SceneManager();
		sceneManager->initScene();

		//Game Loop
		while (openGLLoader->isActive())
		{
			openGLLoader->handleEvents();
			update();
			renderer->render(openGLLoader, sceneManager->getGameObjects(), postProcessor, sceneManager->getMainCamera(), sceneManager->getMainLight());
		}
		cleanUp();
	}

	return 0;
}