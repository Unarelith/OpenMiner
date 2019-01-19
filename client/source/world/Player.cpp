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

#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>

#include "BlockType.hpp"
#include "ClientWorld.hpp"
#include "GameKey.hpp"
#include "Player.hpp"

Player *Player::s_instance = nullptr;

Player::Player(gk::Camera &camera) : m_camera(camera) {
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
	if(gk::GamePad::isKeyPressed(GameKey::Jump) && !m_isJumping) {
		m_isJumping = true;
		m_velocity.y = m_jumpSpeed;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Fly)) {
		m_velocity.y = 0.1;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Sneak)) {
		m_velocity.y = -0.1;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Up))        move(0.0f);
	else if(gk::GamePad::isKeyPressed(GameKey::Down)) move(180.0f);

	if(gk::GamePad::isKeyPressed(GameKey::Left))       move(-90.0f);
	else if(gk::GamePad::isKeyPressed(GameKey::Right)) move(90.0f);

	if (gk::GamePad::isKeyPressed(GameKey::Left)  && gk::GamePad::isKeyPressed(GameKey::Up))   move(-45.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Right) && gk::GamePad::isKeyPressed(GameKey::Up))   move(45.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Left)  && gk::GamePad::isKeyPressed(GameKey::Down)) move(-135.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Right) && gk::GamePad::isKeyPressed(GameKey::Down)) move(135.0f);
}

void Player::updatePosition(const ClientWorld &world) {
	if (!Config::isFlyModeEnabled) {
		m_velocity.y -= m_gravity; // Gravity

		if (m_velocity.y < -m_jumpSpeed) // Jump max accel
			m_velocity.y = -m_jumpSpeed;
	}

	if (!Config::isNoClipEnabled)
		checkCollisions(world);

	m_x += m_velocity.x;
	m_y += m_velocity.y;
	m_z += m_velocity.z;

	m_camera.setPosition(m_x, m_y, m_z);

	if (m_velocity.x != 0 || m_velocity.y != 0 || m_velocity.z != 0)
		m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());

	m_velocity.x = 0;
	m_velocity.z = 0;

	if (Config::isFlyModeEnabled)
		m_velocity.y = 0;
}

// FIXME: Use AABB for more precision
void Player::checkCollisions(const ClientWorld &world) {
	const float PLAYER_HEIGHT = 1.8;
	float eyeheight = m_y + PLAYER_HEIGHT - 1.4;
	// testPoint(world, glm::vec3(m_x, m_y, m_z), m_velocity);
	testPoint(world, glm::vec3(m_x - 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_z - 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x + 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_z - 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x - 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_z + 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x + 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_z + 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x - 0.2, eyeheight - 0.4, m_z - 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x + 0.2, eyeheight - 0.4, m_z - 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x - 0.2, eyeheight - 0.4, m_z + 0.2), m_velocity);
	testPoint(world, glm::vec3(m_x + 0.2, eyeheight - 0.4, m_z + 0.2), m_velocity);
}

bool passable(const ClientWorld &world, float x, float y, float z) {
	u32 block = world.getBlock(x, y, z);
	return !block || block == 8 || block == BlockType::Flower;
}

void Player::testPoint(const ClientWorld &world, glm::vec3 pos, glm::vec3 &speed) {
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

