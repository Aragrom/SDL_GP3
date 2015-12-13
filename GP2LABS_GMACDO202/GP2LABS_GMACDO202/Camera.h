#ifndef Camera_h
#define Camera_h

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

class Camera :public Component
{
public:
	
	Camera();
	~Camera();

	void Update();

	void SetLook(float x, float y, float z);
	vec3 GetLook();

	void SetUp(float x, float y, float z);

	void SetNearClip(float near);
	void SetFarClip(float far);
	void SetFOV(float FOV);

	void SetAspectRatio(float ratio);

	mat4& GetView();
	mat4& GetProjection();

	void MoveCamera(string strDir);
	void RotateCamera(int iDir);

protected:

private:

	vec3 m_v3LookAt;
	vec3 m_v3Up;

	mat4 m_m4View;
	mat4 m_m4Projection;

	float m_fNearClip;
	float m_fFarClip;
	float m_fFOV;
	float m_fAspectRatio;

	float m_fAngle;
	float m_fSinX;
	float m_fCosZ;
	float m_fX;
	float m_fY;
	float m_fZ;
};

#endif