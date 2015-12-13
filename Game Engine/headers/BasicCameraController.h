#ifndef BasicCameraController_h
#define BasicCameraController_h


#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Component.h"
#include <vector>

class BasicCameraController :public Camera
{
public:

	BasicCameraController();			// Constructor
	~BasicCameraController();			// Deconstructor

	// Setters
	void setLookingAtTarget(bool b);
	void setCanMove(bool b);
	void setLookAngle(float f);

	// Getters
	bool getLookingAtTarget();

	void addGameObjectToTargets(GameObject *go);	// Add GameObject the camera can target specifically
	void iterateThroughTargets();					// Iterates through count that tracks the object the camera is look at

	void moveCamera(std::string strDir);			// Moves the Camera taking a string which defines the direction to move
	void rotateCameraHorizontal(int iDir);			// Rotates the camera based on an int representing LEFT or RIGHT
	//void rotateCameraVertical(int iDir);			// !- Needs Constructed -!

	void rotateObject(int iDir);					// Rotates GameObject targetted by camera
	void moveObject(std::string strDir);			// Move GameObject based on string representing direction

	int getDir();									// Gets current moving direction

	void update();
	void Destroy();

protected:

private:

	bool m_bLookAtTarget;								// Stores value if the camera is targetting objects
	char m_cAxis;										// The axis currently being moved

	int m_iDir;											// Current Direction moving

	std::vector<GameObject*> m_vTargetGameObjects;		// Vector holding pointers to GameObjects targetable in the scene
	GameObject* m_goTarget;								// Pointer to GameObject holding TARGET
	int m_iTargetIndex;									// Iterator for Targetable GameObject Vector List

	bool m_bCanMove;

	float m_fAngle;										// Current Basic Camera Controller rotation
	float m_fSinX;										// Value for Rotating Horizontally
	float m_fCosZ;										// Value for Rotating Horizontally

	float m_fObjectMoveSpeed;							// Basic Camera Controller MOVE SPEED
	float m_fObjectRotateSpeed;							// Basic Camera Controller ROTATE SPEED
};

#endif