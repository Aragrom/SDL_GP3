/*
Class Description: Component that handles shaders
and textures. Governs the "Look" of objects
*/
#include "Material.h"
#include "Shader.h"

/*
Constructor
*/
Material::Material()
{
	m_gluiShaderProgram = -1;
	m_strType = "Material";
	m_v4AmbientColour = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_v4DiffuseColour = vec4(0.75f, 0.75f, 0.75f, 1.0f);
	m_v4SpecularColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_fSpecularPower = 200.0f;
}

/*
Deconstructor
*/
Material::~Material()
{

}

/*
Memory Clean Up
*/
void Material::Destroy()
{
	glDeleteProgram(m_gluiShaderProgram);
}

/*
glUseProgram(SHADER_PROGRAM)
*/
void Material::Bind()
{
	glUseProgram(m_gluiShaderProgram);
}

/*
Process:
[1] Load Vertex/Fragment shader from file
[2] Bind shaders to Shader Program
[3] Check for errors
[4] Releases any memory used
[5] Define attributes position in memory
*/
bool Material::LoadShader(const std::string& strVSFilename,
	const std::string& strFSFilename)
{
	//Intialise Vertex shader from file
	GLuint gluiVertexShaderProgram = 0;
	gluiVertexShaderProgram = LoadShaderFromFile(const_cast<std::string&>(strVSFilename),
		VERTEX_SHADER);

	//Intialise Fragment shader from file
	GLuint gluiFragmentShaderProgram = 0;
	gluiFragmentShaderProgram = LoadShaderFromFile(const_cast<std::string&>(strFSFilename),
		FRAGMENT_SHADER);

	//Attach shaders to shader program
	m_gluiShaderProgram = glCreateProgram();
	glAttachShader(m_gluiShaderProgram, gluiVertexShaderProgram);
	glAttachShader(m_gluiShaderProgram, gluiFragmentShaderProgram);
	glLinkProgram(m_gluiShaderProgram);
	//Check For errors during linking process
	CheckForLinkErrors(m_gluiShaderProgram);

	//Memory Clean up, delete the VS & FS Programs
	glDeleteShader(gluiVertexShaderProgram);
	glDeleteShader(gluiFragmentShaderProgram);

	//Attributes position in memory
	glBindAttribLocation(m_gluiShaderProgram, 0, "vertexPosition");
	glBindAttribLocation(m_gluiShaderProgram, 1, "vertexNormals");
	glBindAttribLocation(m_gluiShaderProgram, 2, "vertexTexCoords");
	glBindAttribLocation(m_gluiShaderProgram, 3, "vertexColour");

	return true;
}

/* 
Returns the location of a uniform variable
*/
GLint Material::GetUniformLocation(const std::string& strName)
{
	return glGetUniformLocation(m_gluiShaderProgram, strName.c_str());
}

/*
Read only
*/
vec4& Material::GetAmbientColour()
{
	return m_v4AmbientColour;
}

void Material::SetAmbientColour(float fR, float fG, float fB, float fA)
{
	m_v4AmbientColour = vec4(fR, fG, fB, fA);
}

/*
Read only
*/
vec4& Material::GetDiffuseColour()
{
	return m_v4DiffuseColour;
}

void Material::SetDiffuseColour(float fR, float fG, float fB, float fA)
{
	m_v4DiffuseColour = vec4(fR, fG, fB, fA);
}

/*
Read only
*/
vec4& Material::GetSpecularColour()
{
	return m_v4SpecularColour;
}

/*
Write only
*/
void Material::SetSpecularColour(float r, float g, float b, float a)
{
	m_v4SpecularColour = vec4(r, g, b, a);
}

/*
Read only
*/
float Material::GetSpecularPower()
{
	return m_fSpecularPower;
}

/*
Write only
*/
void Material::SetSpecularPower(float fPower)
{
	m_fSpecularPower = fPower;
}