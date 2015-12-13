#include "GameConstants.h"

#include "BoxCollider.h"

//Getters
float BoxCollider::getLength() { return m_fLength; }
float BoxCollider::getWidth() {	return m_fWidth; }
float BoxCollider::getHeight() { return m_fHeight; }
std::vector<vec3*> BoxCollider::getColliderVertices() { return m_colliderVertices; }

//Setters
void BoxCollider::setLength(float x) { m_fLength = x; }
void BoxCollider::setHeight(float y) { m_fHeight = y; }
void BoxCollider::setWidth(float z) { m_fWidth = z; }

BoxCollider::BoxCollider(float fWidth, float fHeight, float fLength)
{
	m_Active = true;
	m_Type = "Collider";
	m_ColliderType = "BoxCollider";
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fLength = fLength;
}

void BoxCollider::createColliderVertices()
{
	vec3 v3Center = m_Parent->getTransform()->getPosition();

	if (m_fLength != 0 && m_fWidth != 0 && m_fWidth != 0)
	{
		//1
		vec3* v3_1 = new vec3(v3Center.x - (m_fLength / 2),
			v3Center.y + (m_fHeight / 2),
			v3Center.z - (m_fWidth / 2));

		m_colliderVertices.push_back(v3_1);
		//2
		vec3* v3_2 = new vec3(v3Center.x - (m_fLength / 2),
			v3Center.y - (m_fHeight / 2),
			v3Center.z - (m_fWidth / 2));

		m_colliderVertices.push_back(v3_2);
		//3
		vec3* v3_3 = new vec3(v3Center.x + (m_fLength / 2),
			v3Center.y + (m_fHeight / 2),
			v3Center.z - (m_fWidth / 2));

		m_colliderVertices.push_back(v3_3);
		//4
		vec3* v3_4 = new vec3(v3Center.x + (m_fLength / 2),
			v3Center.y - (m_fHeight / 2),
			v3Center.z - (m_fWidth / 2));

		m_colliderVertices.push_back(v3_4);
		//5
		vec3* v3_5 = new vec3(v3Center.x - (m_fLength / 2),
			v3Center.y + (m_fHeight / 2),
			v3Center.z + (m_fWidth / 2));

		m_colliderVertices.push_back(v3_5);
		//6
		vec3* v3_6 = new vec3(v3Center.x - (m_fLength / 2),
			v3Center.y - (m_fHeight / 2),
			v3Center.z + (m_fWidth / 2));

		m_colliderVertices.push_back(v3_6);
		//7
		vec3* v3_7 = new vec3(v3Center.x + (m_fLength / 2),
			v3Center.y + (m_fHeight / 2),
			v3Center.z + (m_fWidth / 2));

		m_colliderVertices.push_back(v3_7);
		//8
		vec3* v3_8 = new vec3(v3Center.x + (m_fLength / 2),
			v3Center.y - (m_fHeight / 2),
			v3Center.z + (m_fWidth / 2));

		m_colliderVertices.push_back(v3_8);
	}
	else { std::cout << "Box Collider - Dimension = 0 avoided division" << std::endl; }

}

bool BoxCollider::withinColliderCheckPointer(vec3 *pos)
{
	vec3 v3Center = m_Parent->getTransform()->getPosition();
	v3Center = v3Center + vec3(0.0f, m_fHeight / 2.0f, 0.0f);

	if (pos->x > (v3Center.x - (m_fLength / 2)) && pos->x < (v3Center.x + (m_fLength / 2)));
	else return false;

	if (pos->y >(v3Center.y - (m_fHeight / 2)) && pos->y < (v3Center.y + (m_fHeight / 2)));
	else return false;

	if (pos->z >(v3Center.z - (m_fWidth / 2)) && pos->z < (v3Center.z + (m_fWidth / 2)));
	else return false;

	return true;
}

bool BoxCollider::withinColliderCheck(vec3 pos)
{
	vec3 v3Center = m_Parent->getTransform()->getPosition();
	v3Center = v3Center + vec3(0.0f, m_fHeight/2.0f, 0.0f);

	if (pos.x >(v3Center.x - (m_fLength / 2)) && pos.x < (v3Center.x + (m_fLength / 2)));
	else return false;

	if (pos.y >(v3Center.y - (m_fHeight / 2)) && pos.y < (v3Center.y + (m_fHeight / 2)));
	else return false;

	if (pos.z >(v3Center.z - (m_fWidth / 2)) && pos.z < (v3Center.z + (m_fWidth / 2)));
	else return false;

	return true;
}

bool BoxCollider::withinColliderCheck(BoxCollider *boxCollider)
{
	bool bResult = false;

	for (auto iter = boxCollider->m_colliderVertices.begin(); iter != boxCollider->m_colliderVertices.end(); iter++)
	{
		//Check for Collision
		if(!bResult) bResult = withinColliderCheckPointer((*iter));
	}

	return bResult;
}