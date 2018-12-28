/*
 * =====================================================================================
 *
 *       Filename:  Player.cpp
 *
 *    Description:
 *
 *        Created:  25/12/2018 22:59:17
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include "GamePad.hpp"
#include "Mouse.hpp"
#include "Player.hpp"
#include "World.hpp"

Player *Player::s_instance = nullptr;

Player::Player() {
	m_x = 0;
	m_y = 18;
	m_z = 20;

	m_angleH = 90.0;
	m_angleV = 0.01;

	m_camera.setPosition(m_x, m_y, m_z);
	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void Player::turnH(float angle) {
	m_angleH += angle;

	while(m_angleH >= 180.0f) {
		m_angleH -= 360.0f;
	}
	while(m_angleH < -180.0f) {
		m_angleH += 360.0f;
	}

	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void Player::turnV(float angle) {
	m_angleV += angle;

	if(89.9f < m_angleV) {
		m_angleV = 89.9f;
	}
	else if(-89.9f > m_angleV) {
		m_angleV = -89.9f;
	}

	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void Player::move(float direction) {
	direction += m_angleH;

	m_velocity.x = 0.04f * cos(direction * RADIANS_PER_DEGREES);
	m_velocity.z = 0.04f * sin(direction * RADIANS_PER_DEGREES);
}

void Player::processInputs() {
	if(GamePad::isKeyPressed(GameKey::Jump) && !m_isJumping) {
		m_isJumping = true;
		m_velocity.y = m_jumpSpeed;
	}

	if(GamePad::isKeyPressed(GameKey::Fly)) {
		m_velocity.y = 0.1;
	}

	if(GamePad::isKeyPressed(GameKey::Sneak)) {
		m_velocity.y = -0.1;
	}

	if(GamePad::isKeyPressed(GameKey::Up))      move(0.0f);
	else if(GamePad::isKeyPressed(GameKey::Down)) move(180.0f);

	if(GamePad::isKeyPressed(GameKey::Left))      move(-90.0f);
	else if(GamePad::isKeyPressed(GameKey::Right)) move(90.0f);

	if (GamePad::isKeyPressed(GameKey::Left)  && GamePad::isKeyPressed(GameKey::Up))   move(-45.0f);
	if (GamePad::isKeyPressed(GameKey::Right) && GamePad::isKeyPressed(GameKey::Up))   move(45.0f);
	if (GamePad::isKeyPressed(GameKey::Left)  && GamePad::isKeyPressed(GameKey::Down)) move(-135.0f);
	if (GamePad::isKeyPressed(GameKey::Right) && GamePad::isKeyPressed(GameKey::Down)) move(135.0f);
}

glm::mat4 Player::updatePosition(const World &world) {
	m_velocity.y -= m_gravity; // Gravity

	if (m_velocity.y < -m_jumpSpeed) // Jump max accel
		m_velocity.y = -m_jumpSpeed;

	checkCollisions(world);

	m_x += m_velocity.x;
	m_y += m_velocity.y;
	m_z += m_velocity.z;

	m_camera.setPosition(m_x, m_y, m_z);

	if (m_velocity.x != 0 || m_velocity.y != 0 || m_velocity.z != 0)
		m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());

	m_velocity.x = 0;
	m_velocity.z = 0;

	return m_camera.getViewMatrix().getMatrix();
}

// FIXME: Use AABB for more precision
void Player::checkCollisions(const World &world) {
	const float PLAYER_HEIGHT = 1.8;
	float m_eyeheight = m_y + PLAYER_HEIGHT - 1;
	// testPoint(world, glm::vec3(m_x, m_y, m_z), m_velocity);
	testPoint(world, glm::vec3(m_x - 0.2, m_eyeheight - PLAYER_HEIGHT - 0.4, m_z - 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x + 0.2, m_eyeheight - PLAYER_HEIGHT - 0.4, m_z - 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x - 0.2, m_eyeheight - PLAYER_HEIGHT - 0.4, m_z + 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x + 0.2, m_eyeheight - PLAYER_HEIGHT - 0.4, m_z + 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x - 0.2, m_eyeheight - 0.4, m_z - 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x + 0.2, m_eyeheight - 0.4, m_z - 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x - 0.2, m_eyeheight - 0.4, m_z + 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x + 0.2, m_eyeheight - 0.4, m_z + 0.2), m_velocity);
}

bool passable(const World &world, float x, float y, float z) {
	u32 block = world.getBlock(x, y, z);
	return !block || block == 8;
}

void Player::testPoint(const World &world, glm::vec3 pos, glm::vec3 &speed) {
	// FIXME: Temporary fix, find the real problem!!!
	if (pos.x < 0) --pos.x;
	if (pos.y < 1) --pos.y;
	if (pos.z < 0) --pos.z;

	if(!passable(world, pos.x + speed.x, pos.y, pos.z)) speed.x = 0;
	if(!passable(world, pos.x, pos.y, pos.z + speed.z)) speed.z = 0;
	if(!passable(world, pos.x, pos.y + speed.y, pos.z)) {
		if(speed.y < 0 && m_isJumping) m_isJumping = false;
		speed.y = 0;
	}
}

