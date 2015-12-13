#ifndef _SHAPE_H
#define _SHAPE_H

#include "Vertex.h"
#include "GameObject.h"
#include "GL\glew.h"

class Shape
{

public:

	Shape();
	void initShape(GameObject * go);
	void generateMesh(GameObject * go, GLuint indices[], Vertex vertices[], int iIdenices, int iVerticies);
	void TEST(GameObject *go);

	glm::vec3 m_topLeftFront;
	glm::vec3 m_bottomLeftFront;
	glm::vec3 m_topRightFront;
	glm::vec3 m_bottomRightFront;
	glm::vec3 m_topLeftBack;
	glm::vec3 m_topRightBack;
	glm::vec3 m_bottomLeftBack;
	glm::vec3 m_bottomRightBack;

	glm::vec3 m_Faces[24];
	GLuint m_Indices[36];

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_colourList;
};

#endif

