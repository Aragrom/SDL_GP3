#include "Light.h"

/*
Constructor
*/
Light::Light()
{
	m_v4DiffuseColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_v4SpecularColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_v3LightDirection = vec3(0.0f, 0.5f, 0.5f);
	m_strType = "Light";
}

/*
Deconstructor
*/
Light::~Light()
{

}

/*
Read only
*/
vec4& Light::GetSpecularColour()
{
	return m_v4SpecularColour;
}

/*
Write only
*/
void Light::SetSpecularColour(float fR, float fG, float fB, float fA)
{
	m_v4SpecularColour = vec4(fR, fG, fB, fA);
}

/*
Read only
*/
vec4& Light::GetDiffuseColour()
{
	return m_v4DiffuseColour;
}

/*
Write only
*/
void Light::SetDiffuseColour(float fR, float fG, float fB, float fA)
{
	m_v4DiffuseColour = vec4(fR, fG, fB, fA);
}

/*
Read only
*/
vec3& Light::GetDirection()
{
	return m_v3LightDirection;
}

/*
Write only
*/
void Light::SetDirection(float fX, float fY, float fZ)
{
	m_v3LightDirection = vec3(fX, fY, fZ);
}