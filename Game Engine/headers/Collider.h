#ifndef COLLIDER_H
#define COLLIDER_H

#include "GameConstants.h"

class Collider : public Component
{
public:

	//Setters
	void setColliderType(std::string strType);

	//Getters
	std::string getColliderType();

	Collider();
	~Collider();

	std::string m_ColliderType;

private:

};

#endif

