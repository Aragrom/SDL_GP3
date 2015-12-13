/*
==========================================================================
Enemy.h
==========================================================================
*/

#ifndef _enemy_h
#define _enemy_h

#include "GameConstants.h"
#include <vector>

class Enemy
{
public:

	//Setter
	void setGameObject(GameObject *go) { m_gameObject = go; }
	void setTarget(GameObject *go) { m_goTarget = go; }
	void setDistanceToEngage(float f) { m_fDistanceToEngage = f; }

	Enemy();
	~Enemy();

	void update();
	float calculateDistanceToTarget();
	void calculateDirectionToMove();
	void move();


private:
	
	GameObject *m_gameObject;
	GameObject *m_goTarget;

	enum eMoveDirections { NORTH, EAST, SOUTH, WEST };
	bool m_bIsMoving;
	int m_iDirection;
	float m_fDistanceToEngage;

};

#endif
