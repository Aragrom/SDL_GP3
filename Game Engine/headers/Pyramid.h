/*
==========================================================================
Pyramid.h
==========================================================================
*/

#ifndef _PYRAMID_H
#define _PYRAMID_H

#include "windowOGL.h"
#include "glm\glm.hpp"
#include "glx\glext.h"
#include "wglext.h"
#include <vector>
#include "Colours.h"
#include "Shapes.h"

class Pyramid : public Shapes
{
public:

	Pyramid();	// Constructor
	virtual void render(float rotAngle);			// Render to the active buffer
	virtual void initialise(Texture theTexture);	// Initialise Pyramid with a texture

private:

	// Store position of vertices
	glm::vec3 m_Top;
	glm::vec3 m_frontBottomLeft;
	glm::vec3 m_frontBottomRight;

	glm::vec3 m_rightBottomLeft;
	glm::vec3 m_rightBottomRight;

	glm::vec3 m_leftBottomLeft;
	glm::vec3 m_leftBottomRight;

	glm::vec3 m_backBottomLeft;
	glm::vec3 m_backBottomRight;

	glm::vec3 m_Faces[24];		// Array of Vector 3 representing Faces
	GLuint m_Indices[18];		// Array of GLuint representing the pyramid's indices

	std::vector<glm::vec3> m_vertices;		// Stores Vector of vector 3 representing the pyramid's vertices
	std::vector<glm::vec3> m_colourList;	// Stores Vector of vector 3 representing the pyramid's colour List
	GLuint m_VertexBufferObjects[3];		// Vertex Buffer Object

};

#endif