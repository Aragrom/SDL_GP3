/*
==========================================================================
Shapes.h
==========================================================================
*/

#ifndef _SHAPES_H
#define _SHAPES_H

#include "windowOGL.h"
#include "glm\glm.hpp"
#include "glx\glext.h"
#include "wglext.h"
#include <vector>
#include "Colours.h"
#include "Texture.h"

class Shapes : public windowOGL
{
public:

	// Getters
	float getRotAngle();

	// Setters
	void setRotAngle(float rotAngle);

	Shapes();	// Constructor
	virtual void render(float rotAngle) = 0;			// VIRTUAL Render to active buffer
	void prepare(float rAngle);							// Set rotation angle for shape
	virtual void initialise(Texture theTexture) = 0;	// VIRTUAL initialise Shape with a texture

	PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
	PFNGLBINDBUFFERPROC glBindBuffer = NULL;
	PFNGLBUFFERDATAPROC glBufferData = NULL;

private:

	float m_rotAngle;		// Shape's rotation property
};

#endif