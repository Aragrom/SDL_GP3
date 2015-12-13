/*
==================================================================================
cSphere.cpp
==================================================================================
*/

/*#include "cSphere.h"


Sphere::cSphere()
{

}

Sphere::cSphere(GLdouble dRadius, GLint dSlices, GLint dStacks)
{
	sRadius = dRadius;
	sSlices = dSlices;
	sStacks = dStacks;
}

void Sphere::initialiseTexture(cTexture theTexture)
{
	quad = gluNewQuadric();

	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, theTexture.getTexture());

}

void Sphere::initialiseTexture(GLint theTextureID, glm::vec3 thePosition, glm::vec3 theTranslation)
{
	quad = gluNewQuadric();
	m_TextureID = theTextureID;
	m_Position = thePosition;
	m_Translation = theTranslation;

	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, m_TextureID);

}*/