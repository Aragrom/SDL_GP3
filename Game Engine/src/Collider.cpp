// Author Graham Alexander MacDonald and Adrian Lis
#include <iostream>
#include "GameObject.h"
#include "Transform.h"

#include <glm/glm.hpp>
using glm::vec3;

#include "Collider.h"

Collider::Collider()
{
	m_Active = true;
	m_Type = "BoxCollider";
}

Collider::~Collider()
{

}

float Collider::getLength()
{
	return m_fLength;
}

float Collider::getWidth()
{
	return m_fWidth;
}

float Collider::getHeight()
{
	return m_fHeight;
}

void Collider::setLength(float x)
{
	m_fLength = x;
}

void Collider::setHeight(float y)
{
	m_fHeight = y;
}

void Collider::setWidth(float z)
{
	m_fWidth = z;
}

void Collider::setCentre(vec3 v3Centre)
{
	m_v3Centre = v3Centre;
}

bool Collider::checkForCollision(vec3 pos)
{
	//vec3 v3Centre = m_Parent->getTransform()->getPosition();

	if (pos.x > (m_v3Centre.x - (m_fLength / 2)) && pos.x < (m_v3Centre.x + (m_fLength / 2)));
	else return false;

	if (pos.y >(m_v3Centre.y - (m_fHeight / 2)) && pos.y < (m_v3Centre.y + (m_fHeight / 2)));
	else return false;

	if (pos.z >(m_v3Centre.z - (m_fWidth / 2)) && pos.z < (m_v3Centre.z + (m_fWidth / 2)));
	else return false;

	return true;
}



