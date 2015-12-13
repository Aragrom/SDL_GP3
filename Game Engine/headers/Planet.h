/*
==========================================================================
Enemy.h
==========================================================================
*/

#ifndef _planet_h
#define _planet_h

#include "GameConstants.h"

class GameObject;

class Planet
{
public:

	//Setter
	void setGameObject(GameObject *go) { m_gameObject = go; }

	Planet(float fOrbitDistance);	// Constructor
	~Planet();						// Deconstructor

	void update();	
	void rotate();

private:

	GameObject *m_gameObject;
	float m_fOrbitDistance;
	float m_fAngle;						// Current rotation
	float m_fSinX;						// Value for Rotating
	float m_fCosZ;						// Value for Rotating
};

#endif
