#include "Planet.h"

Planet::Planet(float fOrbitDistance) 
{
	m_fOrbitDistance = fOrbitDistance;
	m_fAngle = 0.0f;
	m_fSinX = 0.0f;
	m_fCosZ = 0.0f;
}

Planet::~Planet() 
{

}

void Planet::update() 
{
	rotate();
}

void Planet::rotate() 
{
	m_fAngle += 0.01f;
	m_fSinX = sin(m_fAngle);
	m_fCosZ = -cos(m_fAngle);

	m_gameObject->getTransform()->setPosition(m_fSinX * m_fOrbitDistance, 0.0f, m_fCosZ * m_fOrbitDistance);
}