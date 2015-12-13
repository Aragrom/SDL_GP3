#ifndef Transform_h
#define Transform_h

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

class Transform :public Component
{
public:

	Transform();
	~Transform();

	void Update();

	void SetPosition(float fX, float fY, float fZ);
	void SetRotation(float fX, float fY, float fZ);
	void SetScale(float fX, float fY, float fZ);

	vec3& GetPosition();
	vec3& GetScale();
	vec3& GetRotation();
	mat4& GetModel();

protected:

private:

	vec3 m_v3Position;
	vec3 m_v3Scale;
	vec3 m_v3Rotation;
	mat4 m_m4Model;

};


#endif