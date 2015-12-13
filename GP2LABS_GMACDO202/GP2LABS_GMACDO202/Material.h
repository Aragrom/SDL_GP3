#ifndef Material_h
#define Material_h

#include <GL/glew.h>

#include <glm/glm.hpp>
using glm::vec4;

#include <string>

#ifdef __APPLE__
#include <SDL2/SDL_opengl.h>
#include <OpenGL/glu.h>
#elif WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#endif

#include "Component.h"

using namespace std;

class Material :public Component
{
public:

	Material();
	~Material();

	void Destroy();
	void Bind();

	bool LoadShader(const string& strVSFilename,
		const string& strFSFilename);

	GLint GetUniformLocation(const string& strName);

	vec4& GetAmbientColour();
	void SetAmbientColour(float fR, float fG, float fB, float fA);

	vec4& GetDiffuseColour();
	void SetDiffuseColour(float fR, float fG, float fB, float fA);

	vec4& GetSpecularColour();
	void SetSpecularColour(float fR, float fG, float fB, float fA);

	float GetSpecularPower();
	void SetSpecularPower(float fPower);

protected:

private:

	GLuint m_gluiShaderProgram;
	vec4 m_v4AmbientColour;
	vec4 m_v4DiffuseColour;
	vec4 m_v4SpecularColour;
	float m_fSpecularPower;
};

#endif