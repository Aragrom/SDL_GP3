/*
Class Description: Container for components
*/

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"

/*
Constructor
*/
GameObject::GameObject()
{
	m_Transform = NULL;
	m_Mesh = NULL;
	m_Material = NULL;
	m_Camera = NULL;
	m_Light = NULL;
	m_goParent = NULL;
	m_strName = "GameObject";
}

/*
Deconstructor
*/
GameObject::~GameObject()
{

}

#pragma region Properties
/*Add parameter component to vGameObjects
setting its parent to this GameObject*/
void GameObject::AddComponent(Component *c)
{
	c->SetParent(this);
	m_vComponents.push_back(c);
}

void GameObject::AddChild(GameObject *go)
{
	go->SetParent(this);
	m_vChildGameObjects.push_back(go);
}

/*Write only*/
void GameObject::SetName(const std::string& strName)
{
	m_strName = strName;
}

/*Read only*/
const std::string& GameObject::GetName()
{
	return m_strName;
}

/* Calls AddComponent(component)
Write Only*/
void GameObject::SetTransform(Transform * t)
{
	m_Transform = t;
	AddComponent(t);
}

/* Calls AddComponent(component)
Write only*/
void GameObject::SetMesh(Mesh * mesh)
{
	m_Mesh = mesh;
	AddComponent(mesh);
}

/* Calls AddComponent(component)
Write only*/
void GameObject::SetMaterial(Material * mat)
{
	m_Material = mat;
	AddComponent(mat);
}

/* Calls AddComponent(component)
Write only*/
void GameObject::SetCamera(Camera * cam)
{
	m_Camera = cam;
	AddComponent(cam);
}

/* Calls AddComponent(component)
Write only*/
void GameObject::SetParent(GameObject *comp)
{
	m_goParent = comp;
}

/* Write only */
void GameObject::SetLight(Light *light)
{
	m_Light = light;
	AddComponent(light);
} 

/*Read only*/
Light * GameObject::GetLight()
{
	return m_Light;
}

/*Read only*/
Transform * GameObject::GetTransform()
{
	return m_Transform;
}

/*Read only*/
Mesh * GameObject::GetMesh()
{
	return m_Mesh;
}

/*Read only*/
Material * GameObject::GetMaterial()
{
	return m_Material;
}

/*Read only*/
Camera * GameObject::GetCamera()
{
	return m_Camera;
}

/*Read only*/
GameObject* GameObject::GetParent()
{
	return m_goParent;
}

int GameObject::GetChildCount()
{
	return m_vChildGameObjects.size();
}

GameObject * GameObject::GetChild(int iIndex)
{
	if (iIndex < m_vChildGameObjects.size())
		return m_vChildGameObjects[iIndex];
	else
		return NULL;
}
#pragma endregion

/*For all Components in "vComponents" & call Init()*/
void GameObject::Init()
{
	for (auto iter = m_vComponents.begin();
		iter != m_vComponents.end();
		iter++)
	{
		(*iter)->Init();
	}

	for (auto iter = m_vChildGameObjects.begin();
		iter != m_vChildGameObjects.end();
		iter++)
	{
		(*iter)->Init();
	}

}

/*For all GameObjects in "vGameObjects" call Update()*/
void GameObject::Update()
{
	for (auto iter = m_vComponents.begin();
		iter != m_vComponents.end();
		iter++)
	{
		(*iter)->Update();
	}

	for (auto iter = m_vChildGameObjects.begin();
		iter != m_vChildGameObjects.end();
		iter++)
	{
		(*iter)->Update();
	}

}

/*For all GameObjects in "vGameObjects" call Render()*/
void GameObject::Render()
{
	for (auto iter = m_vComponents.begin();
		iter != m_vComponents.end();
		iter++)
	{
		(*iter)->Render();
	}

	for (auto iter = m_vChildGameObjects.begin();
		iter != m_vChildGameObjects.end();
		iter++)
	{
		(*iter)->Render();
	}

}

/*For all GameObjects in "vGameObjects" call Destroy()*/
void GameObject::Destroy()
{
	auto cIter = m_vComponents.begin();
	while (cIter != m_vComponents.end())
	{
		(*cIter)->Destroy();
		if ((*cIter))
		{
			delete (*cIter);
			(*cIter) = NULL;
			cIter = m_vComponents.erase(cIter);
		}
		else
		{
			cIter++;
		}
	}
	m_vComponents.clear();

	auto goIter = m_vChildGameObjects.begin();
	while (goIter != m_vChildGameObjects.end())
	{
		(*goIter)->Destroy();
		if ((*goIter))
		{
			delete (*goIter);
			(*goIter) = NULL;
			goIter = m_vChildGameObjects.erase(goIter);
		}
		else
		{
			goIter++;
		}
	}
	m_vChildGameObjects.clear();
}
