/*
Class Description: Used to hold visual elements
including Vertex Buffer Objects, Element Buffer
Objects and Vertex Array Objects
*/

#include "Mesh.h"
#include "Vertex.h"

/*
Constructor
*/
Mesh::Mesh()
{
	m_iVertexCount = 0;
	m_iIndexCount = 0;
	m_gluiVBO = 0;
	m_gluiEBO = 0;
	m_gluiVAO = 0;
	m_strType = "Mesh";
}

/*
Deconstructor
*/
Mesh::~Mesh()
{

}

#pragma region Properties
/*
Read only
*/
int Mesh::GetVertexCount()
{
	return m_iVertexCount;
}

/*
Read only
*/
int Mesh::GetIndexCount()
{
	return m_iIndexCount;
}
#pragma endregion

/*
Intialise VAO, VBO & EBO
*/
void Mesh::Init()
{
	//Creat array to hold buffer objects
	glGenVertexArrays(1, &m_gluiVAO);
	//Make array object active
	glBindVertexArray(m_gluiVAO);

	//Create buffer
	glGenBuffers(1, &m_gluiVBO);
	// Make the new VBO active
	glBindBuffer(GL_ARRAY_BUFFER, m_gluiVBO);
	//create buffer
	glGenBuffers(1, &m_gluiEBO);
	//Make the EBO active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gluiEBO);

	//Tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)sizeof(vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)+sizeof(vec3)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)+sizeof(vec3)+sizeof(vec2)));

}

/*
Clean up for memory, deletes VBO, EBO & VAO
*/
void Mesh::Destroy()
{
	glDeleteBuffers(1, &m_gluiVBO);
	glDeleteBuffers(1, &m_gluiEBO);
	glDeleteVertexArrays(1, &m_gluiVAO);
}

/*
Binds buffers to the pipeline
*/
void Mesh::Bind()
{
	glBindVertexArray(m_gluiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_gluiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gluiEBO);

}

/*
Copies data passed by parameter into the VBO
*/
void Mesh::CopyVertexData(int iCount, int iStride, void ** data)
{
	m_iVertexCount = iCount;
	glBindBuffer(GL_ARRAY_BUFFER, m_gluiVBO);
	glBufferData(GL_ARRAY_BUFFER, iCount * iStride, data, GL_STATIC_DRAW);
}

/*
Copies data passed by parameter into EBO
*/
void Mesh::CopyIndexData(int count, int stride, void ** data)
{
	m_iIndexCount = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gluiEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * stride, data, GL_STATIC_DRAW);
}