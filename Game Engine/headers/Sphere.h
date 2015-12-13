/*
==========================================================================
Sphere.h
==========================================================================
*/

#ifndef _SPHERE_H
#define _SPHERE_H

#include "GameConstants.h"
#include "glm\glm.hpp"
#include "glx\glext.h"
#include "wglext.h"
#include "GL\glut.h"
#include "Shapes.h"

class Sphere : public Shapes
{
public:

	Sphere();	// Constructor
	Sphere(GLdouble dRadius, GLint dSlices, GLint dStacks);				// Constructor - Defines Sphere Properties
	virtual void render(float rotAngle);								// Render to the active buffer
	virtual void initialise(Texture theTexture);						// Initialise Cube with a texture
	virtual void initialise(GLint theTextureID, glm::vec3 thePosition, glm::vec3 theTranslation);		// Initialise Cube with a texture that is already created with a position and translation

private:

	GLdouble sRadius;				// Sphere's RADIUS property
	GLint sSlices;					// Sphere's SLICES property
	GLint sStacks;					// Sphere's STACKS property
	glm::vec3 m_Position;			// Sphere's World Position
	glm::vec3 m_Translation;		// Sphere's translation
	GLUquadric *quad;				// Quad to hold the Cube's texture
	GLuint m_TextureID;				// Stores GLuint representing the texture using an ID

};

#endif