#ifndef Mesh_h
#define Mesh_h

#include <GL/glew.h>

#ifdef __APPLE__
#include <SDL2/SDL_opengl.h>
#include <OpenGL/glu.h>
#elif WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#endif

#include "Component.h"

class Mesh :public Component
{
public:

	Mesh();
	~Mesh();

	void Init();
	void Destroy();
	void Bind();

	void CopyVertexData(int iCount, int iStride, void ** data);
	void CopyIndexData(int iCount, int iStride, void ** data);

	int GetVertexCount();
	int GetIndexCount();

protected:

private:

	int m_iVertexCount;
	int m_iIndexCount;
	GLuint m_gluiVBO;
	GLuint m_gluiEBO;
	GLuint m_gluiVAO;

};

#endif