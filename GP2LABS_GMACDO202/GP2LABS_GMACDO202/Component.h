#ifndef Component_h
#define Component_h

#include <string>
using namespace std;

class GameObject;

class Component
{
public:

	Component();
	virtual ~Component();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	const string& GetType();

	bool IsActive();
	void ToggleActive();

	void SetParent(GameObject * object);

protected:

	string m_strType;
	bool m_bActive;
	GameObject * m_goParent;

private:

};
#endif