/*
 * =====================================================================================
 *
 *       Filename:  ClientPlayer.cpp
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
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "GameKey.hpp"

ClientPlayer *ClientPlayer::s_instance = nullptr;

ClientPlayer::ClientPlayer(gk::Camera &camera) : m_camera(camera) {
	// FIXME: Warning: Duplicated in ServerCommandHandler.hpp
	m_x = 12;
	m_y = 18;
	m_z = 12;

	m_angleH = -90.0;
	m_angleV = 0.01;

	m_hitbox = gk::FloatBox{-0.2, -1.8, -0.2, 0.4, 1.8, 0.4};

	m_camera.setPosition(m_x, m_y, m_z);
	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void ClientPlayer::turnH(float angle) {
	m_angleH += angle;

	while(m_angleH >= 180.0f) {
		m_angleH -= 360.0f;
	}
	while(m_angleH < -180.0f) {
		m_angleH += 360.0f;
	}

	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void ClientPlayer::turnV(float angle) {
	m_angleV += angle;

	if(89.9f < m_angleV) {
		m_angleV = 89.9f;
	}
	else if(-89.9f > m_angleV) {
		m_angleV = -89.9f;
	}

	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void ClientPlayer::move(float direction) {
	direction += m_angleH;

	m_velocity.x = 0.04f * cos(direction * RADIANS_PER_DEGREES);
	m_velocity.z = 0.04f * sin(direction * RADIANS_PER_DEGREES);
}

void ClientPlayer::processInputs() {
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

	if (gk::GamePad::isKeyPressed(GameKey::Sprint)) {
		m_velocity.x *= 1.5;
		m_velocity.z *= 1.5;
	}
}

void ClientPlayer::updatePosition(const ClientWorld &world) {
	ClientChunk *chunk = (ClientChunk *)world.getChunkAtBlockPos(m_x, m_y - 0.2, m_z);
	if (!Config::isFlyModeEnabled && chunk && chunk->isInitialized()) {
		m_velocity.y -= m_gravity; // Gravity

		if (m_velocity.y < -m_jumpSpeed) // Jump max accel
			m_velocity.y = -m_jumpSpeed;
	}

	if (!Config::isNoClipEnabled)
		checkCollisions(world);

	if (!Config::isFlyModeEnabled && m_velocity.y != 0) {
		m_velocity.x *= 0.75f;
		m_velocity.z *= 0.75f;
	}

	m_x += m_velocity.x;
	m_y += m_velocity.y;
	m_z += m_velocity.z;

	m_camera.setPosition(m_x, m_y, m_z);
	Player::setPosition(m_x, m_y, m_z);

	if (m_velocity.x != 0 || m_velocity.y != 0 || m_velocity.z != 0)
		m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());

	m_velocity.x = 0;
	m_velocity.z = 0;

	if (Config::isFlyModeEnabled)
		m_velocity.y = 0;
}

void ClientPlayer::checkCollisions(const ClientWorld &world) {
	for (float x = m_x + m_hitbox.x ; x <= m_x + m_hitbox.x + m_hitbox.width + 0.1f ; x += 0.2f) {
		for (float y = m_y + m_hitbox.y ; y <= m_y + m_hitbox.y + m_hitbox.height + 0.1f ; y += 0.9f) {
			for (float z = m_z + m_hitbox.z ; z <= m_z + m_hitbox.z + m_hitbox.depth + 0.1f ; z += 0.2f) {
				if (x == m_x + m_hitbox.x || x == m_x + m_hitbox.x + m_hitbox.width
				 || y == m_y + m_hitbox.y || y == m_y + m_hitbox.y + m_hitbox.height
				 || z == m_z + m_hitbox.z || z == m_z + m_hitbox.z + m_hitbox.depth)
					testPoint(world, x, y, z, m_velocity);
			}
		}
	}
}

bool passable(const ClientWorld &world, float x, float y, float z) {
	u32 block = world.getBlock(x, y, z);
	return !block || block == 8 || block == BlockType::Flower;
}

void ClientPlayer::testPoint(const ClientWorld &world, float x, float y, float z, glm::vec3 &speed) {
	// FIXME: Temporary fix, find the real problem!!!
	if (x < 0) --x;
	if (y < 1) --y;
	if (z < 0) --z;

	if(!passable(world, x + speed.x, y, z)) speed.x = 0;
	if(!passable(world, x, y, z + speed.z)) speed.z = 0;
	if(!passable(world, x, y + speed.y, z)) {
		if(speed.y < 0 && m_isJumping) m_isJumping = false;
		speed.y = 0;
	}
}

