//by Graham Alexander MacDonald and Adrian Lis
#ifndef Collider_h
#define Collider_h

#include "Component.h"

#include <glm/glm.hpp>
using glm::vec3;

class Collider : public Component
{
public:

	Collider();
	~Collider();

	bool checkForCollision(vec3 pos);

	float getLength();
	float getHeight();
	float getWidth();

	void setLength(float x);
	void setHeight(float y);
	void setWidth(float z);

	void setCentre(vec3 v3Centre);

private:

	float m_fLength;
	float m_fWidth;
	float m_fHeight;

	vec3 m_v3Centre;
};

#endif

