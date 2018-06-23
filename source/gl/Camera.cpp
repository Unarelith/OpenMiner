/*
 * =====================================================================================
 *
 *       Filename:  Camera.cpp
 *
 *    Description:
 *
 *        Created:  16/12/2014 12:21:19
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "World.hpp"

Camera::Camera() {
	m_x = 0;
	m_y = 18;
	m_z = 20;

	m_angleH = 90.0;
	m_angleV = 0.01;

	update();
}

void Camera::turnH(float angle) {
	m_angleH += angle;

	while(m_angleH >= 180.0f) {
		m_angleH -= 360.0f;
	}
	while(m_angleH < -180.0f) {
		m_angleH += 360.0f;
	}
}

void Camera::turnV(float angle) {
	m_angleV += angle;

	if(89.9f < m_angleV) {
		m_angleV = 89.9f;
	}
	else if(-89.9f > m_angleV) {
		m_angleV = -89.9f;
	}
}

void Camera::move(float direction) {
	direction += m_angleH;

	m_velocity.x = 0.04f * cos(direction * RADIANS_PER_DEGREES);
	m_velocity.z = 0.04f * sin(direction * RADIANS_PER_DEGREES);
}

glm::mat4 Camera::processInputs(const World &world) {
	// if(Mouse::getDX() != 0 || Mouse::getDY() != 0) {
	// 	turnH(Mouse::getDX() * 0.2f);//0.02);
	// 	turnV(-Mouse::getDY() * 0.2f);//0.02);
	// 	// m_viewMatrix = update();
	// }

	if(Keyboard::isKeyPressed(Keyboard::Space) && !m_isJumping) {
		m_isJumping = true;
		m_velocity.y = m_jumpSpeed;
	}
	if(Keyboard::isKeyPressed(Keyboard::X)) {
		m_velocity.y = 0.1;
	}
	if(Keyboard::isKeyPressed(Keyboard::LeftShift)) {
		m_velocity.y = -0.1;
	}

	if(Keyboard::isKeyPressed(Keyboard::Z))      move(0.0f);
	else if(Keyboard::isKeyPressed(Keyboard::S)) move(180.0f);

	if(Keyboard::isKeyPressed(Keyboard::Q))      move(-90.0f);
	else if(Keyboard::isKeyPressed(Keyboard::D)) move(90.0f);

	if (Keyboard::isKeyPressed(Keyboard::Q) && Keyboard::isKeyPressed(Keyboard::Z)) move(-45.0f);
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::Z)) move(45.0f);
	if (Keyboard::isKeyPressed(Keyboard::Q) && Keyboard::isKeyPressed(Keyboard::S)) move(-135.0f);
	if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S)) move(135.0f);

	m_velocity.y -= m_gravity; // Gravity

	if (m_velocity.y < -m_jumpSpeed) // Jump max accel
		m_velocity.y = -m_jumpSpeed;

	checkCollisions(world);

	m_x += m_velocity.x;
	m_y += m_velocity.y;
	m_z += m_velocity.z;

	if (m_velocity.x != 0 || m_velocity.y != 0 || m_velocity.z != 0)
		update();

	m_velocity.x = 0;
	m_velocity.z = 0;

	return m_viewMatrix;
}

// FIXME: Use AABB for more precision
void Camera::checkCollisions(const World &world) {
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

void Camera::update() {
	m_viewMatrix = glm::lookAt(glm::vec3(m_x, m_y, m_z),
	                           glm::vec3(pointTargetedX(), pointTargetedY(), pointTargetedZ()),
	                           glm::vec3(0, 1, 0));
}

bool passable(const World &world, float x, float y, float z) {
	u32 block = world.getBlock(x, y, z);
	return !block || block == 8;
}

void Camera::testPoint(const World &world, glm::vec3 pos, glm::vec3 &speed) {
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

