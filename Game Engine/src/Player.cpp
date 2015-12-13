#include "Player.h"

Player::Player() 
{
	m_iLives = 2;
	m_bIsFalling = true;
	m_fFallVelocity = 0.01f;
}

Player::~Player()
{

}

void Player::update()
{
	if (m_bIsFalling)
	{
		moveUsingGravity();
		checkOutOfBounds();
	}
}

void Player::setIsFalling(bool b)
{
	if (m_bIsFalling != b) m_fFallVelocity = 0.01f;
	m_bIsFalling = b;
}

void Player::moveUsingGravity()
{
	vec3 v3 = m_gameObject->getTransform()->getPosition();
	vec3 v3Camera = m_gameObject->getCamera()->getLookAt();

	v3 = v3 - vec3(0.0f, 0.01f * m_fFallVelocity, 0.0f);
	m_gameObject->getTransform()->setPosition(v3.x, v3.y, v3.z);

	v3Camera = v3Camera - vec3(0.0f, 0.01f * m_fFallVelocity, 0.0f);
	m_gameObject->getCamera()->setLook(v3Camera.x, v3Camera.y, v3Camera.z);

	m_fFallVelocity += 0.05f;
}

void Player::checkOutOfBounds() 
{
	vec3 v3PlayerPosition = m_gameObject->getTransform()->getPosition();
	if (v3PlayerPosition.y < -10.0f)
	{
		m_gameObject->getTransform()->setPosition(V3_START_POSITION.x, V3_START_POSITION.y, V3_START_POSITION.z);
		m_gameObject->getCamera()->setLook(V3_START_POSITION.x, V3_START_POSITION.y, V3_START_POSITION.z - 1.0f);
		static_cast<BasicCameraController*>(m_gameObject->getCamera())->setLookAngle(0.0f);
		std::cout << "RESET POSITION" << std::endl;
	}
}