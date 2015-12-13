#ifndef BoxCollider_h
#define BoxCollider_h

#include "GameConstants.h"

class BoxCollider : public Collider
{
public:

	//Getters
	float getLength();								// Get Length of Box
	float getHeight();								// Get Height of Box
	float getWidth();								// Get Width of Box
	std::vector<vec3*> getColliderVertices();		// Get X, Y and Z for each Vertice as a vector

	//Setters
	void setLength(float x);						// Set Length of Box
	void setHeight(float y);						// Set Height of Box
	void setWidth(float z);							// Set Width of Box

	BoxCollider(float fWidth, float fHeight, float fLength); //Constructor

	void createColliderVertices();							// Creates vertices based on Transform Components Position (as a center)
	bool withinColliderCheckPointer(vec3 *pos);				// Check pointer for vector 3 in Box Collider's boundings
	bool withinColliderCheck(vec3 pos);						// Check for vector 3 in Box Collider's boundings
	bool withinColliderCheck(BoxCollider *boxCollider);		// Takes a Box Collider, Using its vertices checks each one within the Box Collider's Boundings

private:

	std::vector<vec3*> m_colliderVertices;		// Holds vertices for Box Collider
	float m_fLength;							// Length of Box
	float m_fWidth;								// Width of Box
	float m_fHeight;							// Height of Box
};
#endif
