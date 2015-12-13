#ifndef Component_h
#define Component_h

#include <string>

class GameObject;

class Component
{
public:

    Component();	// Constructor
    virtual ~ Component();	// Deconstructor
    
	// VIRTUAL functions
    virtual void init();    
    virtual void update();    
    virtual void render();    
    virtual void destroy();
    
	// Getters
    const std::string& getType();
    const std::string& getName();
	GameObject * getParent();    
    bool isActive();

	// Setters
	void setParent(GameObject * object);
	void setName(const std::string& name);
    
    void toggleActive();	// Based on current state alternate

protected:

    std::string m_Type;			// Stores Component Type property
    std::string m_Name;			// Stores String property for component name
    bool m_Active;				// Component Active State
    GameObject * m_Parent;		// GameObject that owns the component
};

#endif
