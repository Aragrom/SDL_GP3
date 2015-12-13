/*
Class Description: Base for all component
*/
#include "Component.h"

/*
Constructor
*/
Component::Component()
{
	m_strType = "Component";
	m_bActive = true;
}

/*
Deconstructor
*/
Component::~Component()
{

}

#pragma region Properties
/*
Read Only
*/
const std::string& Component::GetType()
{
	return m_strType;
}

/*
Read Only
*/
bool Component::IsActive()
{
	return m_bActive;
}

/*
Write Only
*/
void Component::ToggleActive()
{
	m_bActive = !m_bActive;
}

/*
Write Only
*/
void Component::SetParent(GameObject *go)
{
	m_goParent = go;
}
#pragma endregion

void Component::Init()
{
	//Virtual
}

void Component::Update()
{
	//Virtual
}

void Component::Render()
{
	//Virtual
}

void Component::Destroy()
{
	//Virtual
}