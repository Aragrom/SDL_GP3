#ifndef _SPHERE_H
#define _SPHERE_H

#include "GameConstants.h"
//#include "windowOGL.h"
//#include "glm\glm.hpp"
//#include "glx\glext.h"
//#include "wglext.h"
//#include "GL\glut.h"
//#include "cShapes.h"
//#include "Texture.h"

class Texture;

class Sphere
{
public:

	Sphere(Mesh *mesh, float radius, unsigned int rings, unsigned int sectors);
	void formatVertexData();
	int getVertexCount();

private:

	Vertex * vertexPosition;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLushort> *indices;
};

#endif
