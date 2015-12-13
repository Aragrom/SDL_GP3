#ifndef GameObject_h
#define GameObject_h

#include <string>
#include <vector>

using namespace std;

class Component;
class Transform;
class Mesh;
class Material;
class Camera;
class Light;

class GameObject
{
public:

	GameObject();
	~GameObject();

	void Init();
	void Update();
	void Render();
	void Destroy();

	void AddComponent(Component* component);
	void AddChild(GameObject* component);
	void SetParent(GameObject* goParent);

	void SetName(const string& strName);
	void SetTransform(Transform* transform);
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	void SetCamera(Camera* camera);	
	void SetLight(Light *light);

	GameObject* GetParent();
	const string& GetName();
	GameObject* GetChild(int iIndex);
	int GetChildCount();

	Transform* GetTransform();
	Mesh* GetMesh();
	Material* GetMaterial();
	Camera* GetCamera();
	Light* GetLight();

protected:

private:

	vector<Component*> m_vComponents;
	string m_strName;

	Transform *m_Transform;
	Mesh *m_Mesh;
	Material *m_Material;
	Camera *m_Camera;
	Light *m_Light;

	GameObject *m_goParent;
	vector<GameObject*> m_vChildGameObjects;

};


#endif