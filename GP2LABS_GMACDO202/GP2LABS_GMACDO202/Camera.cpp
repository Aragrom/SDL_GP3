/*
Class Definition: Calculates view and perspective matrix
*/

#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include  <math.h>

/*
Constructor
*/
Camera::Camera()
{
	m_strType = "Camera";
	m_v3LookAt = vec3(0.0f, 0.0f, 0.0f);
	m_v3Up = vec3(0.0f, 1.0f, 0.0f);
	m_fNearClip = 0.1f;
	m_fFarClip = 100.0f;
	m_fFOV = 45.0f;
	m_fAspectRatio = 16.0f / 9.0f;
	m_m4View = mat4();
	m_m4Projection = mat4();

	m_fAngle = 0.0f;
	m_fSinX = 0.0f,
	m_fCosZ = -1.0f;
	m_fX = 0.0f,
	m_fY = 0.0f;
	m_fZ = 0.0f;
}

/*
Deconstructor
*/
Camera::~Camera()
{

}

#pragma region Properties
/*
Write Only
*/
void Camera::SetLook(float fX, float fY, float fZ)
{
	m_v3LookAt = vec3(fX, fY, fZ);
}

/*
Read Only
*/
vec3 Camera::GetLook()
{
	return m_v3LookAt;
}

/*
Write Only
*/
void Camera::SetUp(float fX, float fY, float fZ)
{
	m_v3Up = vec3(fX, fY, fZ);
}

/*
Write Only
*/
void Camera::SetNearClip(float fNear)
{
	m_fNearClip = fNear;
}

/*
Write Only
*/
void Camera::SetFarClip(float fFar)
{
	m_fFarClip = fFar;
}

/*
Write Only
*/
void Camera::SetFOV(float fFOV)
{
	m_fFOV = fFOV;
}

/*
Write Only
*/
void Camera::SetAspectRatio(float fRatio)
{
	m_fAspectRatio = fRatio;
}

/*
Read Only
*/
mat4& Camera::GetView()
{
	return m_m4View;
}

/*
Read Only
*/
mat4& Camera::GetProjection()
{
	return m_m4Projection;
}

/*
Moves camera forward and backward using
a direction
*/
void Camera::MoveCamera(string strDir)
{
	if (strDir == "forward")
	{

		m_fX += m_fSinX * 0.1f;
		m_fZ += m_fCosZ * 0.1f;
	}

	if (strDir == "backward")
	{

		m_fX -= m_fSinX * 0.1f;
		m_fZ -= m_fCosZ * 0.1f;
	}

	if (strDir == "up")
	{
		m_fY += 0.01f;
	}

	if (strDir == "down")
	{
		m_fY -= 0.01f;
	}

	m_goParent->GetTransform()->SetPosition(m_fX, m_fY, m_fZ);
	m_v3LookAt = vec3(m_fX + m_fSinX, m_fY, m_fZ + m_fCosZ);
}

/*
Rotates the camera around the Y axis using a direction
*/
void Camera::RotateCamera(int iDir)
{
	if (iDir < 0)
	{
		m_fAngle -= 0.001f;
		m_fSinX = sin(m_fAngle);
		m_fCosZ = -cos(m_fAngle);
	}

	if (iDir > 0)
	{
		m_fAngle += 0.001f;
		m_fSinX = sin(m_fAngle);
		m_fCosZ = -cos(m_fAngle);
	}

	m_v3LookAt = vec3(m_fX + m_fSinX, m_fY, m_fZ + m_fCosZ);
}
#pragma endregion

/*
Updates matrices for Projection and view
using parents>transform>position
*/
void Camera::Update()
{
	//get the position from the transform
	vec3 v3Position = m_goParent->GetTransform()->GetPosition();

	m_m4Projection = glm::perspective(m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
	m_m4View = glm::lookAt(v3Position, m_v3LookAt, m_v3Up);
}