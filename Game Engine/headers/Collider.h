#ifndef COLLIDER_H
#define COLLIDER_H

#include "GameConstants.h"

// Base class for colliders
class Collider : public Component
{
public:

	//Setters
	void setColliderType(std::string strType) { m_ColliderType = strType; }
	void setCollisionDetected(bool b) { m_bCollisionDetected = b; }
	void setOffSet(vec3 v3) { m_v3OffSet = v3; }

	//Getters
	std::string getColliderType() { return m_ColliderType; }
	bool getCollisionDetected() { return m_bCollisionDetected; }
	vec3 getOffSet() { return m_v3OffSet; }

	Collider();		// Constructor
	~Collider();	// Deconstructor

private:

	vec3 m_v3OffSet;
	bool m_bCollisionDetected;
	std::string m_ColliderType;	// String representing Collider type
};

#endif

