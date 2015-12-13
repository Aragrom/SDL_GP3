#ifndef BoxCollider_h
#define BoxCollider_h

#include "GameConstants.h"

class BoxCollider : public Collider
{
public:

	//Getters
	float getLength();
	float getHeight();
	float getWidth();
	std::vector<vec3*> getColliderVertices();

	//Setters
	void setLength(float x);
	void setHeight(float y);
	void setWidth(float z);

	BoxCollider(float fWidth, float fHeight, float fLength); //Constructor

	void createColliderVertices();
	bool withinColliderCheckPointer(vec3 *pos);
	bool withinColliderCheck(vec3 pos);
	bool withinColliderCheck(BoxCollider *boxCollider);

private:

	std::vector<vec3*> m_colliderVertices;
	float m_fLength;
	float m_fWidth;
	float m_fHeight;
};
#endif
