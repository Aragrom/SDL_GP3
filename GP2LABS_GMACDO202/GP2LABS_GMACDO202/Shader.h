#ifndef Shader_h
#define Shader_h

#include <GL/glew.h>

#ifdef WIN32
#include <SDL_opengl.h>
#elif __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum SHADER_TYPE
{
	VERTEX_SHADER = GL_VERTEX_SHADER,
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

GLuint LoadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType);
GLuint LoadShaderFromFile(const string& strFileName, SHADER_TYPE enumShaderType);

bool CheckForLinkErrors(GLuint gluiProgram);
bool CheckForCompilerErrors(GLuint gluiShaderProgram);
#endif