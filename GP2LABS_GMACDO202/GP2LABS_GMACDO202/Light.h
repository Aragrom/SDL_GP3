#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
using glm::vec4;
using glm::vec3;

#include "Component.h"

class Light :public Component
{
public:

	Light();
	~Light();

	vec4& GetDiffuseColour();
	void SetDiffuseColour(float fR, float fG, float fB, float fA);

	vec4& GetSpecularColour();
	void SetSpecularColour(float fR, float fG, float fB, float fA);

	vec3& GetDirection();
	void SetDirection(float fX, float fY, float fZ);

protected:

private:

	vec4 m_v4DiffuseColour;
	vec4 m_v4SpecularColour;

	vec3 m_v3LightDirection;
};

#endif