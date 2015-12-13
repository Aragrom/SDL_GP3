/*
Class Description: represents a model transform,
contains variables for position, scale and rotation
*/

#include "Transform.h"
#include "GameObject.h"

/*
Constructor
*/
Transform::Transform()
{
	m_v3Position = vec3(0.0f, 0.0f, 0.0f);
	m_v3Rotation = vec3(0.0f, 0.0f, 0.0f);
	m_v3Scale = vec3(1.0f, 1.0f, 1.0f);
	m_m4Model = mat4();
}

/*
Deconstructor
*/
Transform::~Transform()
{

}

#pragma region Properties
/*
Write only
*/
void Transform::SetPosition(float fX, float fY, float fZ)
{
	m_v3Position = vec3(fX, fY, fZ);
}

/*
Write only
*/
void Transform::SetRotation(float fX, float fY, float fZ)
{
	m_v3Rotation = vec3(fX, fY, fZ);
}

/*
Write only
*/
void Transform::SetScale(float fX, float fY, float fZ)
{
	m_v3Scale = vec3(fX, fY, fZ);
}

/*
Read only
*/
vec3& Transform::GetPosition()
{
	return m_v3Position;
}

/*
Read only
*/
vec3& Transform::GetScale()
{
	return m_v3Scale;
}

/*
Read only
*/
vec3& Transform::GetRotation()
{
	return m_v3Rotation;
}

/*
Read only
*/
mat4& Transform::GetModel()
{
	return m_m4Model;
}
#pragma endregion

/*
Updates m4Model by using current translate, scale
and rotate mapped onto a mat4 variable
*/
void Transform::Update()
{
	// Get translate
	mat4 m4Translate = glm::translate(mat4(1.0f), m_v3Position);

	// Get scale
	mat4 m4Scale = glm::scale(mat4(1.0f), m_v3Scale);

	// x rotation
	mat4 m4rotationX = glm::rotate(mat4(1.0f),
		m_v3Rotation.x,
		vec3(1.0f, 0.0f, 0.0f));

	// y rotation
	mat4 m4rotationY = glm::rotate(mat4(1.0f),
		m_v3Rotation.y,
		vec3(0.0f, 1.0f, 0.0f));

	// z rotation
	mat4 m4rotationZ = glm::rotate(mat4(1.0f),
		m_v3Rotation.z,
		vec3(0.0f, 0.0f, 1.0f));

	//Put x, y and z rotation together
	mat4 m4Rotation = m4rotationX*m4rotationY*m4rotationZ;

	// Put translate, rotation and scale together
	m_m4Model = m4Translate*m4Rotation*m4Scale;

	if (m_goParent->GetParent())
	{
		m_m4Model = m_goParent->GetParent()->GetTransform()->GetModel()*m_m4Model;
	}
}