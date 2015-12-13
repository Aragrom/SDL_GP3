#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

#define GLM_FORCE_RADIANS
//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "Game Engine : Graham MacDonald"

// Additional OpenGL
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <gl/GLU.h>

// Windows & OpenGL 
#include <stdlib.h>
#include <Windows.h>
#include "GL\glut.h"
#include <iostream>
#include <string>
#include <ctime>

// Image Texture loading library
#include "IL\il.h"
#include "IL\ilu.h"
#include "IL\ilut.h"

// Model Loading Library
#include "glm-0.3.2\glm\glm.h"

// Font
#include "FTGL\ftgl.h"

// Maths functions
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\geometric.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\quaternion.hpp"
//using glm::mat4;
//using glm::vec4;
//using glm::vec3;

// STL Container & Algorithms
#include <vector>
#include <map>
#include <algorithm>

// Textures
#include "Texture.h"

// System includes
#include "Vertex.h"
#include "Shader.h"
#include "SDLTextureLoader.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Collider.h"
#include "Camera.h"
#include "Light.h"
#include "FBXLoader.h"
#include "FPSCameraController.h"
#include "BasicCameraController.h"
#include "Shape.h"
#include "PostProcessing.h"
#include "ColourFilters.h"
#include "SkyboxMaterial.h"
#include "Input.h"
#include "Timer.h"
#include "OpenGLLoader.h"
#include "renderer.h"
#include "SkyBox.h"
#include "SceneManager.h"
#include "Sphere.h"

struct lightColour4
{
	lightColour4(float _r = 0.0f, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f) : r(_r), g(_g), b(_b), a(_a) {}

	float r;	// Red
	float g;	// Green
	float b;	// Blue
	float a;	// Alpha
};



struct colour3f
{
	colour3f(float _r = 0.0f, float _g = 0.0f, float _b = 0.0f) : r(_r), g(_g), b(_b) {}

	float r;	// Red
	float g;	// Green
	float b;	// Blue
};


struct mdlDimensions {
	float s_mdlWidth, s_mdlheight, s_mdldepth;
};

const float PLAYFIELDX = 100.0f;
const float PLAYFIELDZ = 300.0f;

extern int drawMode;
extern float rotationAngle;
extern float translationZ;
extern bool fire;

//Camera Window Width
const int iWINDOW_WIDTH = 600;
//Camera Window Height
const int iWINDOW_HEIGHT = 480;

enum eGraphicSystem { OpenGL, DirectX };

const std::string ASSET_PATH = "../assets/";
const std::string SHADER_PATH = "shaders/";
const std::string TEXTURE_PATH = "textures/";
const std::string FONT_PATH = "fonts/";
const std::string MODEL_PATH = "models/";

#endif
