/*
==========================================================================
Cube.h
==========================================================================
*/

#ifndef _CUBE_H
#define _CUBE_H

#include "windowOGL.h"
#include "glm\glm.hpp"
#include "glx\glext.h"
#include "wglext.h"
#include <vector>
#include "Colours.h"
#include "Shapes.h"

class Cube : public Shapes
{
public:

	Cube();		// Constructor
	virtual void render(float rotAngle);			// Render to active buffer
	virtual void initialise(Texture theTexture);	// Intialise Cube with a texture
	void loadTextureCoords();						// Load texture indices

private:

	glm::vec3 m_topLeftFront;
	glm::vec3 m_bottomLeftFront;
	glm::vec3 m_topRightFront;
	glm::vec3 m_bottomRightFront;
	glm::vec3 m_topLeftBack;
	glm::vec3 m_topRightBack;
	glm::vec3 m_bottomLeftBack;
	glm::vec3 m_bottomRightBack;

	glm::vec3 m_Faces[24];		// Array of Vector 3 representing Faces
	GLuint m_Indices[36];		// Array of GLuint representing the cubes indices

	std::vector<glm::vec3> m_vertices;		// Stores Vector of vector 3 representing the cubes vertices
	std::vector<glm::vec3> m_colourList;	// Stores Vector of vector 3 representing the cubes colour list
	GLuint m_VertexBufferObjects[3];		// Vertex Buffer Object
	glm::vec2 m_TextureIndices[24];			// Store Array of vec 2 representing texture coordinates
	GLuint m_TextureID;						// Stores GLuint representing the texture using an ID

};

#endif