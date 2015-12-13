#include "GameConstants.h"

//Setter
void Collider::setColliderType(std::string strType) { m_ColliderType = strType; }

//Getter
std::string Collider::getColliderType() { return m_ColliderType; }

Collider::Collider()
{
	m_Active = true;
	m_Type = "Collider";
	m_ColliderType = "";
}

Collider::~Collider()
{

}



